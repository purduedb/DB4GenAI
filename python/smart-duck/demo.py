#!/usr/bin/env python3
from vllm import LLM, SamplingParams
import lancedb
import argparse
from lancedb.pydantic import LanceModel, Vector
from lancedb.embeddings import get_registry
import duckdb
import pyarrow as pa
from collections import defaultdict as dd


def _llm(x):
    global llm, sampling_params
    prompts = x.to_pylist()
    prompts_index = dd(set)
    outputs = [prompt for prompt in prompts]
    for idx, prompt in enumerate(prompts):
        prompts_index[prompt].add(idx)
    _prompts = list(set(prompts))
    _prompts.sort()
    _outputs =  [o.outputs[0].text for o in llm.generate(_prompts, sampling_params)]
    for idx, output in enumerate(_outputs):
        prompt = _prompts[idx]
        for original_idx in prompts_index[prompt]:
            outputs[original_idx] = output
    return pa.chunked_array([outputs])

def _semantic_search(x):
    global vectordb
    vector_table = vectordb.open_table("large-squad")
    queries = x.to_pylist()
    contexts = [vector_table.search(query).limit(3).to_pydantic(Words) for query in queries]
    contexts_str = ['\n'.join([word.text for word in context]) for context in contexts]
    return pa.chunked_array([contexts_str])


def prepare_tomato(con):
    con.sql("create table reviews as select * from 'reviews_demo.csv';")
    con.sql("select * from reviews;").show()
    con.sql("create table movies as select * from 'movies_demo.csv';")
    con.sql("select * from movies;").show()

def prepare_squad(con):
    con.sql("create table squad as select * from 'squad_demo.csv';")
    con.sql("select * from squad;").show()

def read_sql(file_path):
    with open(file_path, 'r') as file:
        file_content = file.read()
    return file_content

if __name__ == "__main__":
    # Parser Related
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--model', default='NousResearch/Llama-2-7b-chat-hf')
    args = parser.parse_args()

    # LLM Related
    llm = LLM(model=args.model, enable_prefix_caching=True)
    sampling_params = SamplingParams(temperature=0.0)
    
    # VectorDB Related
    vectordb = lancedb.connect("/tmp/knowledge")
    embedding_model = get_registry().get("sentence-transformers").create(name="BAAI/bge-large-en-v1.5")
    class Words(LanceModel):
        global embedding_model
        text: str = embedding_model.SourceField()
        vector: Vector(embedding_model.ndims()) = embedding_model.VectorField()
    
    # DB Related
    con = duckdb.connect()
    prepare_tomato(con)
    prepare_squad(con)
    con.create_function('llm', _llm, ['VARCHAR'], 'VARCHAR', type='arrow')
    con.create_function('semantic_search', _semantic_search, ['VARCHAR'], 'VARCHAR', type='arrow')

    # Q1
    input("Q1:")
    con.sql('create table t(a varchar, b varchar);')
    con.sql("insert into t values ('1', '12'), ('2','12');")
    con.sql("select format('reorder concat {} {}',t.a,t.b, true) from t;").show()
    q1 = read_sql('sqlexample/q1.sql')
    con.sql(q1).show()

    # Q2
    debug = input("Q2:")
    if debug == 'debug':
        q2 = read_sql('sqlexample/q2_debug.sql')
    else:
        q2 = read_sql('sqlexample/q2.sql')
    con.sql(q2).show()

    # Q3
    input("Q3:")
    q3 = read_sql('sqlexample/q3.sql')
    con.sql(q3).show()

    # Q4
    input("Q4:")
    q4 = read_sql('sqlexample/q4.sql')
    con.sql(q4).show()

    # Q5
    input("Q5:")
    q5 = read_sql('sqlexample/q5.sql')
    con.sql(q5).show()