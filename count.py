import matplotlib.pyplot as plt
import os 

output_dirname = 'output'

if not os.path.isdir(output_dirname):
    os.mkdir(output_dirname)


class Data:
    def __init__(self, x, y):
        self.x = x
        self.y = y


for root, dirs, files in os.walk(output_dirname):
    for plot_file in files:
        data = dict(x=[], y=[])

        plot_filename, extension = os.path.splitext(plot_file)
        if extension != '.txt':
            continue

        with open('{}/{}'.format(root, plot_file), 'r') as f:
            for i, line in enumerate(f):
                if not ',' in line:
                    continue
                parts = line.split(',')
                data['y'].append(parts[1].split()[-1])
                data['x'].append(parts[0].split()[-1])

        plt.plot(data['x'], data['y'])
        plt.ylabel('Time (seconds)')
        plt.xlabel('Thread count')
        plt.savefig('{}/{}.png'.format(output_dirname, plot_filename), bbox_inches='tight')
        plt.clf()
