import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression

plt.rcParams["font.family"] = 'sans-serif'
matplotlib.rcParams.update({'font.size': 15})
color_list = ['#56A768', '#C34F52', '#8272B1', '#4A71AF', '#CCBB79', '#74BDD2']

def plot_degree(data_name, fname, save_name, x_name, y_name):
    
    plt.figure(figsize=(3.6,2.5), dpi=120)
    
    dir_path = './' + data_name + '/'
    
    print(data_name)
    datapath = dir_path + fname + '_graph.txt'
    data = pd.read_csv(datapath)
    data = data[data[y_name]!=0]
    datax = data[x_name]
    datay = data[y_name]
    plt.scatter(datax, datay, s=40, alpha=0.8, label='data', c=color_list[0])
    
    random_path = dir_path + fname + '_random.txt'
    random_data = pd.read_csv(random_path)
    random_data = random_data[random_data[y_name]!=0]
    randomx = random_data[x_name]
    randomy = random_data[y_name]
    plt.scatter(randomx, randomy, s=40, alpha=0.8, label='random', c=color_list[1])
    
    max_xlim = max(max(datax), max(randomx))
    
    plt.xlabel(x_name)
    plt.ylabel(y_name)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlim([0.8, 10*max_xlim])

    ax = plt.gca()
    ax.tick_params(labelcolor='#4B4B4B', labelsize=11)
    ax.yaxis.set_label_coords(-0.33,0.48) 
    ax.xaxis.set_label_coords(0.33,-0.2)
    postfix = '.jpg'

    plt.tight_layout()
    plt.savefig(save_name + '_' + data_name + '.jpg', bbox_inches='tight')
    plt.show()  
    
def plot_density(data_name, fname, save_name):
    dir_path = './' + data_name + '/'
    
    # Random
    random_path = dir_path + fname + '_random.txt'
    random_data = pd.read_csv(random_path)    
      
    # Data
    path = dir_path + fname + '_graph.txt'
    data = pd.read_csv(path)
    
    print(data_name)    
    plt.figure(figsize=(3.6,2.5), dpi=120)
     
    data_number = data.groupby(['num_hes']).mean()
    data_x = np.array(data_number['num_nodes'])
    data_y = np.array(data_number.index)
    
    random_number = random_data.groupby(['num_hes']).mean()
    random_x = np.array(random_number['num_nodes'])
    random_y = np.array(random_number.index)
    
    min_x = min(min(data_x), min(random_x))
    max_x = max(max(data_x), max(random_x))
    
    # linear regression : slope ~ average density
    reg_data = LinearRegression().fit(data_x.reshape(-1, 1), data_y.reshape(-1, 1))
    reg_datax = np.linspace(min_x, max_x)
    reg_datay = reg_datax * reg_data.coef_[0] + reg_data.intercept_
    plt.plot(reg_datax, reg_datay,c=color_list[0], alpha=0.55, linestyle='--')
    
    reg_random = LinearRegression().fit(random_x.reshape(-1, 1),random_y.reshape(-1, 1))
    reg_randomx = np.linspace(min_x, max_x)
    reg_randomy = reg_randomx * reg_random.coef_[0] + reg_random.intercept_
    plt.plot(reg_randomx, reg_randomy, c=color_list[1], alpha=0.55, linestyle='--')
    
    # scatter plot
    plt.scatter(data_x, data_y, label='data', c=color_list[0], alpha=0.8, s=40)
    plt.scatter(random_x, random_y,label='random', c=color_list[1], alpha=0.8, s=40)
    
    plt.ylabel('# hyperedges')
    plt.xlabel('avg. # nodes')
    
    ax = plt.gca()
    ax.tick_params(labelcolor='#4B4B4B', labelsize=10.5)
    ax.yaxis.set_label_coords(-0.33,0.5) 
    ax.xaxis.set_label_coords(0.5,-0.2)
    
    plt.tight_layout()
    plt.savefig(save_name + '_' + data_name + '.jpg', bbox_inches='tight')
    plt.show()

def plot_overlapness(data_name, fname, save_name):
    dir_path = './' + data_name + '/'
    
    # Random
    random_path = dir_path + fname + '_random.txt'
    random_data = pd.read_csv(random_path)
      
    # Data
    path = dir_path + fname + '_graph.txt'
    data = pd.read_csv(path)
    
    print(data_name)

    plt.figure(figsize=(3.6,2.5), dpi=120)

    data_number = data.groupby(['sum_hyperedge_size']).mean()
    data_x = np.array(data_number['num_nodes'])
    data_y = np.array(data_number.index)
    
    random_number = random_data.groupby(['sum_hyperedge_size']).mean()
    random_x = np.array(random_number['num_nodes'])
    random_y = np.array(random_number.index)
    
    min_x = min(min(data_x), min(random_x))
    max_x = max(max(data_x), max(random_x))
    
    # linear regression : slope ~ average density
    reg_data = LinearRegression().fit(data_x.reshape(-1, 1), data_y.reshape(-1, 1))
    reg_datax = np.linspace(min_x, max_x)
    reg_datay = reg_datax * reg_data.coef_[0] + reg_data.intercept_
    plt.plot(reg_datax, reg_datay,c=color_list[0], alpha=0.55, linestyle='--')
    
    reg_random = LinearRegression().fit(random_x.reshape(-1, 1),random_y.reshape(-1, 1))
    reg_randomx = np.linspace(min_x, max_x)
    reg_randomy = reg_randomx * reg_random.coef_[0] + reg_random.intercept_
    plt.plot(reg_randomx, reg_randomy, c=color_list[1], alpha=0.55, linestyle='--')
    
    # scatter plot
    plt.scatter(data_x, data_y, label='data', c=color_list[0], alpha=0.8, s=40)
    plt.scatter(random_x, random_y,label='random', c=color_list[1], alpha=0.8, s=40)
    
    plt.xlabel('avg. # nodes')
    plt.ylabel('$\sum$ hyperedge size')
    
    ax = plt.gca()
    ax.tick_params(labelcolor='#4B4B4B', labelsize=10.5)
    ax.yaxis.set_label_coords(-0.33,0.37) 
    ax.xaxis.set_label_coords(0.5,-0.2)
            
    plt.tight_layout()
    plt.savefig( save_name + '_' + data_name + '.jpg', bbox_inches='tight')
    plt.show()

def plot_homogeneity(data_name, fname, x_name, y_name, save_name):
    round_num = 0
    
    dir_path = './' + data_name + '/'
    plt.figure(figsize=(3.6,2.5), dpi=120)
    
    # data
    data = pd.read_csv(dir_path + fname + '_graph.txt')
    data = data[data[x_name] != 0.0]
    data[x_name] = data[x_name].round(round_num)
    count_x = data[x_name].value_counts().sort_index()
    
    data_x = count_x.index
    data_y = count_x.values

    plt.scatter(data_x, data_y, s=40, alpha=0.8, c=color_list[0], label='data')
    
    # random
    randomdata = pd.read_csv(dir_path + fname + '_random.txt')
    randomdata = randomdata[randomdata[x_name] != 0.0]
    randomdata[x_name] = randomdata[x_name].round(round_num)
    random_count_x = randomdata[x_name].value_counts().sort_index()

    random_x = random_count_x.index
    random_y = random_count_x.values
    
    plt.scatter(random_x, random_y, s=40, alpha=0.8, c=color_list[1], label='random')

    plt.xlabel(x_name)
    plt.ylabel(y_name)
    plt.xscale('log')
    plt.yscale('log')
        
    ax = plt.gca()
    ax.tick_params(labelcolor='#4B4B4B', labelsize=11)
    ax.yaxis.set_label_coords(-0.33,0.48) 
    ax.xaxis.set_label_coords(0.5,-0.2)
    
    plt.tight_layout()
    plt.savefig( save_name + '_' + data_name + '.jpg', bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    #dataset = ["email-Enron-full", "email-Eu-full", "contact-high", "contact-primary", "NDC-classes-full", "NDC-substances-full", "tags-math", "tags-ubuntu", "threads-math" , "threads-ubuntu", "coauth-DBLP-full", "coauth-geology-full", "coauth-history-full"]
    
    dataset = ["email-Enron-full"]
    for d in dataset:
        plot_degree(data_name=d, fname='pair_degree', save_name = 'pair_deg', x_name='# overlapping hyperedges', y_name = '# node pairs')
        plot_degree(data_name=d, fname='triple_degree', save_name = 'triple_deg', x_name='# overlapping hyperedges', y_name = '# node triples')
        plot_density(data_name=d, fname='egonet_density', save_name = 'egonet_density')
        plot_overlapness(data_name=d, fname='egonet_overlapness', save_name= 'egonet_overlapness')
        plot_homogeneity(data_name=d, fname='hyperedge_homogeneity', x_name='homogeneity', y_name = '# hyperedges', save_name = 'hyperedge_homogeneity')