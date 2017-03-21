import plotly
import sys
import plotly.graph_objs as go
from plotly.offline import download_plotlyjs, init_notebook_mode, plot, iplot


def bar_construction(values, index):
    bar_name = ''
    if (i % 3) == 0:
        bar_name = 'correct'
    else:
        bar_name = 'incorrect'

    bar = go.Bar(
        x = x_labels,
        y = values,
        name = bar_name)

    return bar

graph_name = sys.argv[1]

x_labels = sys.argv[2].split(',')

data = [bar_construction(sys.argv[i].split(','), i)
    for i in range(3, len(sys.argv))]

layout = go.Layout(
    barmode='group'
)

fig = go.Figure(data=data, layout=layout)

plotly.offline.init_notebook_mode(connected=True)
plotly.offline.plot(fig, filename="{}.html".format(graph_name))
