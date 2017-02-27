import sys
import plotly.plotly as py
import plotly.graph_objs as go

def bar_construction(values, index):
    bar_name = ''
    if (i % 2) == 0:
        bar_name = 'correct'
    else:
        bar_name = 'incorrect'

    bar = go.Bar(
        x = x_labels,
        y = values,
        name = bar_name)

    return bar

x_labels = sys.argv[1].split(',')

data = [bar_construction(sys.argv[i].split(','), i)
    for i in range(2, len(sys.argv))]

layout = go.Layout(
    barmode='group'
)

fig = go.Figure(data=data, layout=layout)

py.plot(fig, filename='grouped-bar')

