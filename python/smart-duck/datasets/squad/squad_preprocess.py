#!/usr/bin/env python3
import json
import csv
 
f = open('squad.json')
with open('squad.csv', 'w') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['question', 'context', 'is_impossible'])
    data = json.load(f)
    totali = len(data['data'])
    for i in range(totali):
        totalj = len(data['data'][i]['paragraphs'])
        for j in range(totalj):
            context = data['data'][i]['paragraphs'][j]['context']
            qas = data['data'][i]['paragraphs'][j]['qas']
            totalk = len(qas)
            for k in range(totalk):
                question = qas[k]['question']
                im = qas[k]['is_impossible']
                writer.writerow([str(question),str(context),str(im)])
    f.close()
