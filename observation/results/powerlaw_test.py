import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import powerlaw
from tqdm import tqdm
import argparse

def powerlaw_test(graph_type, test_type):
    if test_type == 'pair':
        fname = 'pair_degree'
        x_name = '# overlapping hyperedges'
        y_name = '# node pairs'

    elif test_type == 'triple':
        fname = 'triple_degree'
        x_name = '# overlapping hyperedges'
        y_name = '# node triples'
    
    elif test_type == 'homogeneity':
        fname = 'hyperedge_homogeneity'
        x_name = None
        y_name = 'homogeneity'    

    dataset = ["email-Enron-full", "email-Eu-full", "contact-high", "contact-primary", "NDC-classes-full", "NDC-substances-full", "tags-math", "tags-ubuntu", "threads-math" , "threads-ubuntu" , "coauth-DBLP-full", "coauth-geology-full", "coauth-history-full"]
    
    df = pd.DataFrame(index=range(len(dataset)), columns=['data name', 'power_law-ll', 'power_law-pval', 'truncated_power_law-ll', 'truncated_power_law-pval','lognormal-ll', 'lognormal-pval'])

    idx = 0
    for data_name in tqdm(dataset):
        dir_path = './' + data_name + '/'
        data_path = dir_path + fname + '_' + graph_type + '.txt'
        print(data_path)
        try:
            data = pd.read_csv(data_path)
        except:
            continue
        data = data[data[y_name]!=0]
        
        # pair or triple degree
        if x_name is not None:
            x = data[x_name]
            y = data[y_name]
            all_data = []
            for _x, _y in zip(x,y):
                all_data += [_x] * round(_y)
            fit = powerlaw.Fit(all_data, verbose=False, discrete=True)
        # hyperedge homogeneity
        else:
            all_data = data[y_name]
            fit = powerlaw.Fit(all_data, verbose=False)
        
        for c in ['power_law', 'truncated_power_law', 'lognormal']:
            try:
                r, p = fit.distribution_compare(c, 'exponential', normalized_ratio=True)
                # positive if the data is more likely in the first distribution
                
                df['data name'][idx] = data_name
                df[c+'-ll'][idx] = r
                df[c+'-pval'][idx] = p
            except:
                continue
            
        idx += 1

    df.to_csv('./' + test_type + '_powerlaw_test_' + graph_type + '.csv', sep=',')
    print(df)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-dtype', '--data_type', type=str, required=True, help='Select the dataset')
    parser.add_argument('-ttype', '--test_type', type=str, required=True, help='Select the observation result to test: [pair, triple, homogeneity]')
    args = parser.parse_args()

    powerlaw_test(args.data_type, args.test_type)
    print("Done " + args.test_type)
