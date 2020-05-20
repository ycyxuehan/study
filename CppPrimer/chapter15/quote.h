#include <string>
#include <iostream>

using namespace std;
class Quote
{
public:
    Quote() = default;
    Quote(const string &book, double sales_price) : bookNo(book), price(sales_price){};
    string isbn() const { return bookNo; };
    virtual double net_price(size_t n) const { return n * price; };
    virtual ~Quote() = default;
    virtual Quote* clone()const & {return new Quote(*this);}
    virtual Quote* clone()&& {return new Quote(std::move(*this));}

private:
    string bookNo;

protected:
    double price = 0.0;
};

class BulkQuote : public Quote
{
public:
    BulkQuote() = default;
    BulkQuote(const string &, double, size_t, double);
    double net_price(size_t) const override;
    BulkQuote *clone()const & {return new BulkQuote(*this);}
    BulkQuote *clone() && {return new BulkQuote(std::move(*this));}
    
private:
    size_t min_qty = 0;
    double discount = 0.0;
};

double print_total(ostream &os, const Quote &item, size_t n)
{
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn() << " #sold: " << n
       << " total due: " << ret << endl;
    return ret;
}

BulkQuote::BulkQuote(const string &book, double p, size_t qty, double disc) : Quote(book, p), min_qty(qty), discount(disc) {}

double BulkQuote::net_price(size_t cnt) const
{
    if(cnt >= min_qty)
        return cnt *(1-discount) * price;
    else
        return cnt * price;
}

