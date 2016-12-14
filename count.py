import matplotlib.pyplot as plt
import os 

output_dirname = 'output'
output_filename = '{}/output.txt'.format(output_dirname)

if not os.path.isdir(output_dirname):
    os.mkdir(output_dirname)

#os.system('./prog > {}'.format(output_filename))


class Data:
    def __init__(self, x, y):
        self.x = x
        self.y = y

data = dict(x=[], y=[])

with open(output_filename, 'r') as f:
    for i, line in enumerate(f):
        parts = line.split(',')
        data['y'].append(parts[1].split()[-1])
        data['x'].append(parts[0].split()[-1])

plt.plot(data['x'], data['y'])
plt.ylabel('Time (seconds)')
plt.xlabel('Thread count')
plt.savefig('{}/local.png'.format(output_dirname), bbox_inches='tight')

