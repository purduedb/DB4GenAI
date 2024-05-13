#!/usr/bin/env python3
import argparse
from vllm import LLM, SamplingParams
import duckdb
import pyarrow as pa
parser = argparse.ArgumentParser()
parser.add_argument('-m', '--model', choices = ['facebook/opt-125m'], default='facebook/opt-125m')
args = parser.parse_args()
llm = LLM(model=args.model)
sampling_params = SamplingParams(temperature=0.8, top_p=0.95)

def _llm(x):
    global llm, sampling_params
    prompts = x.to_pylist()
    outputs =  [o.outputs[0].text for o in llm.generate(prompts, sampling_params)]
    return pa.chunked_array([outputs])

con = duckdb.connect()
con.create_function('llm', _llm, ['VARCHAR'], 'VARCHAR', type='arrow')
con.sql("select llm('what is my name?');").show()
