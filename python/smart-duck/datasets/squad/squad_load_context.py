#!/usr/bin/env python3
import json
import lancedb
from lancedb.pydantic import LanceModel, Vector
from lancedb.embeddings import get_registry
f = open('squad.json')
data = json.load(f)
totali = len(data['data'])
contexts = []
for i in range(totali):
    totalj = len(data['data'][i]['paragraphs'])
    for j in range(totalj):
        context = data['data'][i]['paragraphs'][j]['context']
        contexts.append(str(context))
f.close()
vectordb = lancedb.connect("/tmp/knowledge")
embedding_model = get_registry().get("sentence-transformers").create(name="BAAI/bge-small-en-v1.5")
class Words(LanceModel):
    text: str = embedding_model.SourceField()
    vector: Vector(embedding_model.ndims()) = embedding_model.VectorField()
vector_table = vectordb.create_table("small-squad", schema=Words)
vector_table.add([{"text": context} for context in contexts])
