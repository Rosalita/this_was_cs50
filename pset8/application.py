from cs50 import eprint
import string
import os
import re
from flask import Flask, jsonify, render_template, request

from cs50 import SQL
from helpers import lookup

# Configure application
app = Flask(__name__)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
def index():
    """Render map"""
    return render_template("index.html")


@app.route("/articles")
def articles():
    """Look up articles for geo"""

    geo = request.args.get("geo")
    if not geo:
        raise RuntimeError("no geo provided")
    
    articles = lookup(geo)

    return jsonify(articles)


@app.route("/search")
def search():
    """Search for places that match query"""
    
    q = request.args.get("q") # + "%"

    # strip out any punctuation
    punct = set(string.punctuation)
    q = ''.join(char for char in q if char not in punct)
    w = q.split()
    for i in range(0, len(w)):
        w[i] += "%"
    
    rows = ""
    if len(w) == 1:
        rows = db.execute("SELECT * FROM places WHERE postal_code LIKE :w OR place_name LIKE :w OR admin_name1 LIKE :w", w=w[0])
    elif len(w) == 2 and len(w[1]) == 2:
        rows = db.execute("SELECT * FROM places WHERE postal_code LIKE :w0 OR place_name LIKE :w0 AND admin_code1 LIKE :w1 OR admin_name1 LIKE :w0", w0=w[0], w1=w[1])
    elif len(w) == 2 and len(w[1]) > 2:
        rows = db.execute("SELECT * FROM places WHERE postal_code LIKE :w0 OR place_name LIKE :w0 AND admin_name1 LIKE :w1 OR admin_name1 LIKE :w0", w0=w[0], w1=w[1])
    elif len(w) == 3:
        rows = db.execute("SELECT * FROM places WHERE postal_code LIKE :w0 OR place_name LIKE :w0 AND admin_name1 LIKE :w1 AND country_code LIKE :w2 OR admin_name1 LIKE :w0", w0=w[0], w1=w[1], w2=w[2])

    if len(rows) > 10:
        return jsonify([rows[0], rows[1], rows[2],  rows[3],  rows[4], rows[5], rows[6],  rows[7],  rows[8],  rows[9]])
    else:
        return jsonify(rows)

@app.route("/update")
def update():
    """Find up to 10 places within view"""

    # Ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # Ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # Explode southwest corner into two variables
    sw_lat, sw_lng = map(float, request.args.get("sw").split(","))

    # Explode northeast corner into two variables
    ne_lat, ne_lng = map(float, request.args.get("ne").split(","))

    # Find 10 cities within view, pseudorandomly chosen if more within view
    if sw_lng <= ne_lng:

        # Doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # Crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
                          WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
                          GROUP BY country_code, place_name, admin_code1
                          ORDER BY RANDOM()
                          LIMIT 10""",
                          sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # Output places as JSON
    return jsonify(rows)
