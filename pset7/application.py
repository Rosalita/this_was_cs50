import os
import string

from cs50 import SQL, eprint
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from passlib.apps import custom_app_context as pwd_context

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    getcash = db.execute("SELECT cash FROM users WHERE id = :id", \
                                     id=session["user_id"])
   
    portfolio = db.execute("SELECT * FROM portfolio WHERE userid = :userid", \
                                     userid=session["user_id"])
 
    totalstockvalue = 0
 
    for stock in portfolio:
       
        details = lookup(stock["symbol"])
        if not details:
             return apology("Sorry unable to lookup stock prices")
             
        stock.update({'currentprice': details['price']})
        value = stock['quantity'] * stock['currentprice']
        totalstockvalue += value
    
    allassets = totalstockvalue + getcash[0]["cash"]

    return render_template("index.html", portfolio=portfolio, cash=usd(getcash[0]["cash"]), totalstockvalue=usd(totalstockvalue), allassets=usd(allassets))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    if request.method == "GET":
        return render_template("buy.html")
    
    if request.method == "POST":
       
    #:( buy handles fractional, negative, and non-numeric shares
    #application raised an exception (see log for details)
    
        if not request.form.get("symbol"):
            return apology("Missing Symbol!", 403)
        
        if not request.form.get("shares"):
            return apology("Missing quantity of shares", 403)
        
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("Sorry didnt find that stock")
        
        input = request.form.get("shares")
        
        if any(char.isalpha() for char in input):
            return apology("quantity of shares must be numeric")
        
        specialChars = set(string.punctuation.replace("_", ""))
        if any(char in specialChars for char in input):
            return apology("unable to use special chars in quantity")
        
        quantity = int(request.form.get("shares"))
        
        availableFunds = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
     
        if not availableFunds[0]["cash"] >= (stock["price"] * quantity):
            return apology("Sorry not enough funds to buy")
        
        deductCash = db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash= availableFunds[0]["cash"] - (stock["price"] * quantity ), id = session["user_id"])
   
        checkPortfolio = db.execute("SELECT * FROM portfolio WHERE userid = :userid and symbol = :symbol", \
                                     userid=session["user_id"], symbol=request.form.get("symbol"))
        
        if not checkPortfolio:
            result = db.execute("INSERT INTO portfolio (userid, symbol, quantity, lastpricepaid) \
            VALUES(:userid, :symbol, :quantity, :price)", \
            userid=session["user_id"],  \
            symbol=request.form.get("symbol"), \
            quantity=quantity, \
            price = usd(stock["price"]))
            
  
        else: 
            newquantity = checkPortfolio[0]["quantity"] + quantity
            db.execute("UPDATE portfolio SET quantity=:newquantity \
                        WHERE userid=:userid AND symbol=:symbol", \
                        newquantity=newquantity, userid=session["user_id"], \
                        symbol=stock["symbol"])
        
        #write the history
        history = db.execute("INSERT INTO history (userid, type, symbol, quantity, price) \
        VALUES(:userid, :type, :symbol, :quantity, :price)", \
        userid=session["user_id"],  \
        type="buy", \
        symbol=request.form.get("symbol"), \
        quantity=quantity, \
        price= usd(stock["price"] * quantity))
        
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    history = db.execute("SELECT * FROM history WHERE userid = :userid", \
                          userid=session["user_id"])
    
    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
    
        if not request.form.get("symbol"):
            return apology("Missing Symbol!", 400)
        
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("Sorry didnt find a quote")
    
        return render_template("displayquote.html", symbol=stock["symbol"], price=usd(stock["price"]))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    if request.method == "GET":
        return render_template("register.html")
    
    if request.method == "POST":
        
        if not request.form.get("username"):
            return apology("Missing username!", 400)
        elif not request.form.get("password"):
            return apology("Missing password!", 400)
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("passwords given do not match", 400)
        
        # password must contain 1 letter, 1 number and 1 symbol
        password = request.form.get("password")
        
        if not any(char.isalpha() for char in password):
            return apology("password must contain a letter")
        
        if not any(char.isdigit() for char in password):
            return apology("password must contain a number")
        
        specialChars = set(string.punctuation.replace("_", ""))
        if not any(char in specialChars for char in password):
            return apology("password must contain a special character")
        
        hash=generate_password_hash(request.form.get("password"))
        
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),  hash=hash )
        
        if not result:
            return apology("unable to register user")
            
        session["user_id"] = result
        return redirect("/")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        
        # get all the available symbols that can be sold
        
        symbols = db.execute("SELECT symbol FROM portfolio WHERE userid = :userid", \
                  userid=session["user_id"])
            
        return render_template("sell.html", symbols=symbols)
        
    if request.method == "POST":
    
        if not request.form.get("symbol"):
            return apology("Missing Symbol!", 403)
        
        if not request.form.get("shares"):
            return apology("Missing quantity of shares to sell", 403)
        
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("Sorry didnt find that stock")
        
        quantity = int(request.form.get("shares"))
        
        if (quantity <= 0):
            return apology("can only sell a positive number of shares")
        
        checkPortfolio = db.execute("SELECT * FROM portfolio WHERE userid = :userid and symbol = :symbol", \
                                     userid=session["user_id"], symbol=request.form.get("symbol"))
    
        # if user is trying to sell some stock they dont own
        if not checkPortfolio:
            return apology("Sorry you dont own any of that stock to sell")
            # apologise
        
        # check not not trying to sell more than owned
        if quantity > checkPortfolio[0]["quantity"]:
            return apology("Sorry unable to sell more shares than you own")
        
        else: 
            # give the user some cash
            currentCash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
            addCash = db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash= currentCash[0]["cash"] + (stock["price"] * quantity ), id = session["user_id"])
            
            # sell the stock
            if quantity == checkPortfolio[0]["quantity"]:
                     # if selling everything, delete everything
                  deleteAll = db.execute("DELETE FROM portfolio WHERE userid = :userid and symbol = :symbol", \
                                     userid=session["user_id"], symbol=request.form.get("symbol"))
            
            else:
                
            # if not selling everything, deduct amount sold
                newquantity = checkPortfolio[0]["quantity"] - quantity
                db.execute("UPDATE portfolio SET quantity=:newquantity \
                         WHERE userid=:userid AND symbol=:symbol", \
                         newquantity=newquantity, userid=session["user_id"], \
                         symbol=stock["symbol"])
            
        #write the history
        history = db.execute("INSERT INTO history (userid, type, symbol, quantity, price) \
        VALUES(:userid, :type, :symbol, :quantity, :price)", \
        userid=session["user_id"],  \
        type="sell", \
        symbol=request.form.get("symbol"), \
        quantity=int(request.form.get("shares")), \
        price=stock["price"] * quantity )
     
        return redirect("/")

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)

# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
