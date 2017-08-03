import plotly
import sys
import plotly.graph_objs as go

def bar_construction(values, index):
    bar_name = ''
    if (index == 3):
        bar_name = 'correct'
    elif (index == 4):
        bar_name = 'incorrect'
    else:
        bar_name = 'idle'

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

plotly.offline.plot(fig, filename="{}.html".format(graph_name))