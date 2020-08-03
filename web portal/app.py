import numpy as np
from flask import Flask, request, jsonify, render_template, url_for
import requests, json 
import pickle
import pandas as pd
import folium
import webbrowser

app = Flask(__name__)
model = pickle.load(open('pickle_model.pkl','rb'))


@app.route('/')
def home():
    #return 'Hello World'
    return render_template('index.html')
    #return render_template('index.html')

@app.route('/predict',methods = ['POST'])
def predict():
    LAT = 24.3073759
    LON = 72.3495197
    PRECTOT = 5
    W_Level = 0
    complete_url = "https://api.openweathermap.org/data/2.5/weather?q=&lat=24.3073759&lon=72.3495197&lang=en&appid=3e29e62e2ddf6dd3d2ebd28aed069215"
    response = requests.get(complete_url)
    x = response.json()
    y = x["main"]
    z = x["wind"]
    T2M_MIN = y["temp_min"] - 273.15
    WS10M_MIN = z['speed']
    res = requests.get("https://api.thingspeak.com/channels/1110455/feeds.json?api_key=G3R81PBYLYM5OWV8&results=1")
    x = res.json()
    y = x['feeds']
    S_Moisture = y[-1]['entry_id']
    def delta3(LAT,LON):
        loc = 'There is a high chance of flood'
        title_html = '''<h3 align="center" style="font-size:26px"><b>{}</b></h3>'''.format(loc)   
        o=folium.Map(location=[LAT,LON],zoom_start=11,zoom_control=False,scrollWheelZoom=False,dragging=False)
        folium.Circle(radius=1000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        folium.Circle(radius=5000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        folium.Circle(radius=3000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        o.get_root().html.add_child(folium.Element(title_html))
        o.save('templates/map-with-title.html')
    def delta2(LAT,LON):
        loc = 'There is a moderate chance of flood'
        title_html = '''<h3 align="center" style="font-size:26px"><b>{}</b></h3>'''.format(loc)   
        o=folium.Map(location=[LAT,LON],zoom_start=11,zoom_control=False,scrollWheelZoom=False,dragging=False)
        folium.Circle(radius=1000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        folium.Circle(radius=3000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        o.get_root().html.add_child(folium.Element(title_html))
        o.save('templates/map-with-title.html')
    def delta1(LAT,LON):
        loc = 'There is a slight chance of water overflowing'
        title_html = '''<h3 align="center" style="font-size:26px"><b>{}</b></h3>'''.format(loc)   
        o=folium.Map(location=[LAT,LON],zoom_start=11,zoom_control=False,scrollWheelZoom=False,dragging=False)
        folium.Circle(radius=1000,location=[LAT,LON],color='crimson',fill=False,).add_to(o)
        o.get_root().html.add_child(folium.Element(title_html))
        o.save('templates/map-with-title.html')
    def delta0(LAT,LON):
        loc = 'You are Safe'
        title_html = '''<h3 align="center" style="font-size:26px"><b>{}</b></h3>'''.format(loc)   
        o=folium.Map(location=[LAT,LON],zoom_start=11,zoom_control=False,scrollWheelZoom=False,dragging=False)
        o.get_root().html.add_child(folium.Element(title_html))
        o.save('templates/map-with-title.html')
    def call_func(n,LAT,LON):
        if n==0:
            delta0(LAT,LON)
        if n==1:
            delta1(LAT,LON)
        if n==2:
            delta2(LAT,LON)
        if n==3:
            delta3(LAT,LON)
        
    df = pd.DataFrame(columns=['S_Moisture', 'T2M_MIN', 'WS10M_MIN','W_Level','PRECTOT'])
    df.loc[0]=[S_Moisture,T2M_MIN,WS10M_MIN,W_Level,PRECTOT]
    pred=model.predict(df)
    call_func(pred[0],LAT,LON)
    #webbrowser.open("map-with-title.html",new=10)

    #output = round(prediction[0], 2)
    return render_template('map-with-title.html')

@app.route('/predict_api',methods=['POST'])
def predict_api():
    '''
    For direct API calls trought request
    '''
    data = request.get_json(force=True)
    prediction = model.predict([np.array(list(data.values()))])

    output = prediction[0]
    return jsonify(output)



if __name__ == '__main__':
    app.run(debug=True)