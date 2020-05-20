#include "quote.h"

class DiscQuote : public Quote
{
public:
    DiscQuote() = default;
    DiscQuote(const string &book, double price, size_t qty, double disc) : Quote(book, price), quantity(qty), discount(disc) {}
    double net_price(size_t) const = 0;

protected:
    size_t quantity = 0;
    double discount = 0.0;
};

class Bulk_quote: public DiscQuote {
    public:
    Bulk_quote()=default;
    Bulk_quote(const string& book, double price, size_t qty, double disc):
    DiscQuote(book, price, qty, disc){}
    double net_price(size_t) const override;
};