#!/usr/bin/env python3
from vllm import LLM, SamplingParams
import lancedb
import argparse
from lancedb.pydantic import LanceModel, Vector
from lancedb.embeddings import get_registry
import duckdb
import pyarrow as pa


# distributed
def _llm(x):
    global llm, sampling_params
    prompts = x.to_pylist()
    outputs =  [o.outputs[0].text for o in llm.generate(prompts, sampling_params)]
    return pa.chunked_array([outputs])

def _semantic_search(x):
    global vectordb
    vector_table = vectordb.open_table("words")
    queries = x.to_pylist()
    contexts = [vector_table.search(query).limit(3).to_pydantic(Words) for query in queries]
    contexts_str = ['\n'.join([word.text for word in context]) for context in contexts]
    return pa.chunked_array([contexts_str])


def prepare_vectordb():
    global vectordb
    vectordb.drop_table("words")
    vector_table = vectordb.create_table("words", schema=Words)
    vector_table.add(
        [
            {"text": "hello world"},
            {"text": "goodbye world"}
        ]
    )


if __name__ == "__main__":
    # Parser Related
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--model', choices = ['facebook/opt-125m'], default='facebook/opt-125m')
    args = parser.parse_args()

    # LLM Related
    llm = LLM(model=args.model)
    sampling_params = SamplingParams(temperature=0.8, top_p=0.95)
    
    # VectorDB Related
    vectordb = lancedb.connect("/tmp/knowledge")
    embedding_model = get_registry().get("sentence-transformers").create(name="BAAI/bge-small-en-v1.5")
    class Words(LanceModel):
        global embedding_model
        text: str = embedding_model.SourceField()
        vector: Vector(embedding_model.ndims()) = embedding_model.VectorField()
    prepare_vectordb()
    
    # DB Related
    con = duckdb.connect()
    con.create_function('llm', _llm, ['VARCHAR'], 'VARCHAR', type='arrow')
    con.create_function('semantic_search', _semantic_search, ['VARCHAR'], 'VARCHAR', type='arrow')
    con.sql("select semantic_search('greetings');").show()
