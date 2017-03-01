import plotly
from plotly.graph_objs import Scatter, Layout

plotly.offline.init_notebook_mode(connected=True)

for i in range(1,5):
  plotly.offline.plot({
      "data": [Scatter(x=[i*4, i*3, i*2, i], y=[4*(i+500), 3*(i*3), 2, 1])],
      "layout": Layout(title="hello world")
  }, filename="{}.html".format(i))

