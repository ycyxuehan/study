#include "stdfx.h"
#include "textquery.h"

class QueryBase
{
    friend class Query;

protected:
    virtual ~QueryBase() = default;

private:
    virtual QueryResult eval(const TextQuery &) const = 0;
    virtual string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);

public:
    Query(const string &);
    QueryResult eval(const TextQuery &t) const { return q->eval(t); }
    string rep() const { return q->rep(); }

private:
    Query(shared_ptr<QueryBase> query) : q(query) {}
    shared_ptr<QueryBase> q;
};
ostream &operator<<(ostream &os, const Query &query) { return os << query.rep(); }

class WordQuery : public QueryBase
{
    friend class Query;
    WordQuery(const string &s) : query_word(s) {}
    QueryResult eval(const TextQuery &t) const { return t.query(query_word); }
    string rep() const { return query_word; }
    string query_word;
};

inline Query::Query(const std::string &s) : q(new WordQuery(s)) {}

class NotQuery : public QueryBase
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q) : query(q) {}
    string rep() const { return "~(" + query.rep() + ")"; }
    QueryResult eval(const TextQuery &) const;
    Query query;
};

inline Query operator~(const Query &operand)
{
    return shared_ptr<QueryBase>(new NotQuery(operand));
}

class BinaryQuery : public QueryBase
{
protected:
    BinaryQuery(const Query &l, const Query &r, string s) : lhs(l), rhs(r), opSym(s) {}
    string rep() const { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }
    Query lhs, rhs;
    string opSym;
};

class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query &, const Query &);
    AndQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "&") {}
    QueryResult eval(const TextQuery &) const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<QueryBase>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery
{
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "|") {}
    QueryResult eval(const TextQuery &) const;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
    return shared_ptr<QueryBase>(new OrQuery(lhs, rhs));
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
    ret_lines->insert(right.begin(), right.end());
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
    set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult NotQuery::eval(const TextQuery &text) const
{
    auto result = query.eval(text);
    auto ret_lines = make_shared<set<line_no>>();
    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n)
    {
        if (beg == end || *beg != n)
            ret_lines->insert(n);
        else if (beg != end)
            ++beg;
    }
}