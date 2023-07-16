#pragma once 

#include "chain.h"
#include "xact.h"
#include "post.h"
#include "account.h"
#include "temps.h"

namespace ledger {

using namespace boost::placeholders;

class post_splitter : public item_handler<post_t>
{
public:
  typedef std::map<value_t, posts_list> value_to_posts_map;
  typedef function<void (const value_t&)> custom_flusher_t;

protected:
  value_to_posts_map posts_map;
  post_handler_ptr post_chain;
  report_t& report;
  expr_t& group_by_expr;
  custom_flusher_t preflush_func;
  optional<custom_flusher_t> postflush_func;

public:
  post_splitter(post_handler_ptr _post_chain,
                report_t& _report,
                expr_t& _group_by_expr)
    : post_chain(_post_chain), report(_report),
      group_by_expr(_group_by_expr) {
    preflush_func = bind(&post_splitter::print_title, this, _1);
    TRACE_CTOR(post_splitter, "scope_t&, post_handler_ptr, expr_t");
    }
    virtual ~post_splitter() {
        TRACE_DTOR(post_splitter);
    }

    void set_preflush_func(custom_flusher_t functor) {
        preflush_func = functor;
    }

    void set_postflush_func(custom_flusher_t functor) {
        postflush_func = functor;
    }

    virtual void print_title(const value_t& val);

    virtual void flush();
    virtual void operator()(post_t& post);

    virtual void clear() {
        posts_map.clear();
        post_chain->clear();
        item_handler<post_t>::clear();
    }
};


class ignore_posts : public item_handler<post_t>
{
public:
  virtual void operator()(post_t&) {}
};

class collect_posts : public item_handler<post_t>
{
public:
  std::vector<post_t *> posts;

  collect_posts() : item_handler<post_t>() {
    TRACE_CTOR(collect_posts, "");
  }
  virtual ~collect_posts() {
    TRACE_DTOR(collect_posts);
  }

  std::size_t length() const {
    return posts.size();
  }

  std::vector<post_t *>::iterator begin() {
    return posts.begin();
  }
  std::vector<post_t *>::iterator end() {
    return posts.end();
  }

  virtual void flush() {}
  virtual void operator()(post_t& post) {
    posts.push_back(&post);
  }

  virtual void clear() {
    posts.clear();
    item_handler<post_t>::clear();
  }
};

template <typename Iterator>
class pass_down_posts : public item_handler<post_t>
{
    pass_down_posts();

public:
  pass_down_posts(post_handler_ptr handler, Iterator& iter)
    : item_handler<post_t>(handler) {
    while (post_t * post = *iter) {
      try {
        item_handler<post_t>::operator()(*post);
    }
    catch (const std::exception&) {
        add_error_context(item_context(*post, _("While handling the operation")));
        throw;
    }  
    iter.increment();
   }

   
    }
}
}