#ifndef __BTZIPPER_H
#define __BTZIPPER_H

#include <cstddef>
#include <iostream>
#include <memory>
#include <boost/function.hpp>
#include <boost/bind.hpp>


/* --------------------------------------------------------*/
/* FIRST PART OF SOLUTION */
/* --------------------------------------------------------*/
namespace btzipper {

  namespace implementation {
    
    template < typename T >
    class BaseBTZipper {
    private :
            
      template < typename T2 >
      class TreeNode {
      public:
        T2 value;
        std::shared_ptr< TreeNode > lson;
        std::shared_ptr< TreeNode > rson;
        
        TreeNode(T2 val) : value(val), lson(NULL), rson(NULL) {
        }
        TreeNode(const TreeNode& t) : value(t.value), lson(t.lson), rson(t.rson) {
        }
        TreeNode operator= (const TreeNode& t) {
          value = t.value;
          lson = t.lson;
          rson = t.rson;
          return *this;
        }
      };
      
      template < typename T2 >
      class Context {
      public :
        T2 value;
        bool is_left;
        std::shared_ptr< TreeNode< T2 > > tree;
        std::shared_ptr< Context > context;
        
        Context(T2 val, bool left) : value(val), is_left(left), tree(NULL), context(NULL) {
        }
        Context(const Context& c) : value(c.value), is_left(c.is_left), tree(c.tree), context(c.context) {
        }
        Context operator= (const Context& c) {
          value = c.value;
          is_left = c.is_left;
          tree = c.tree;
          context = c.context;
          return *this;
        }
      };
      
    protected :  
      std::shared_ptr< TreeNode< T > > tree;
      std::shared_ptr< Context< T > > context;
    
    public :
      BaseBTZipper(const T& val) : tree(new TreeNode< T >(val)), context(NULL) {
      }
      BaseBTZipper(const BaseBTZipper& z) : tree(z.tree), context(z.context) {
      }
      BaseBTZipper& operator= (const BaseBTZipper& z) {
        tree = z.tree;
        context = z.context;
        return *this;
      }
      
      bool is_root() const {
        return (context == NULL);
      }
      bool has_left() const {
        return (tree->lson != NULL);
      }
      bool has_right() const{
        return (tree->rson != NULL);
      }
      T get() const {
        return tree->value;
      }
      BaseBTZipper set(T val) const;
      BaseBTZipper move_up() const;
      BaseBTZipper move_left() const;
      BaseBTZipper move_right() const;
      BaseBTZipper push_left(T val) const;
      BaseBTZipper push_right(T val) const;
      BaseBTZipper prune_left() const;
      BaseBTZipper prune_right() const;
    };
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::set(T val) const {
      BaseBTZipper< T > ret(val);
      ret.tree->lson = tree->lson;
      ret.tree->rson = tree->rson;
      ret.context = context;
      return ret;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::push_left(T val) const {
      BaseBTZipper ret(val);
      ret.tree->rson = tree->rson;
      std::shared_ptr< TreeNode< T > > newl(new TreeNode< T >(tree->value));
      newl->lson = tree->lson;
      newl->rson = NULL;
      ret.tree->lson = newl;
      ret.context = context;
      return ret;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::push_right(T val) const {
      BaseBTZipper ret(val);
      std::shared_ptr< TreeNode< T > > newr(new TreeNode< T >(tree->value));
      newr->lson = NULL;
      newr->rson = tree->rson;
      ret.tree->rson = newr;
      ret.tree->lson = tree->lson;
      ret.context = context;
      return ret;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::move_up() const {
      if (!is_root()) {
        BaseBTZipper ret(context->value);
        ret.context = context->context;
        if (!context->is_left) {
          ret.tree->lson = context->tree;
          ret.tree->rson = tree;
        } else {
          ret.tree->rson = context->tree;
          ret.tree->lson = tree;
        }
        return ret;
      } else
        return *this;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::move_left() const {
      if (has_left()) {
        BaseBTZipper ret(tree->lson->value);
        ret.tree->lson = tree->lson->lson;
        ret.tree->rson = tree->lson->rson;
        std::shared_ptr< Context< T > > cont(new Context< T >(tree->value, true));
        cont->context = context;
        cont->tree = tree->rson;
        ret.context = cont;
        return ret;
      } else
        return *this;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::move_right() const {
      if (has_right()) {
        BaseBTZipper ret(tree->rson->value);
        ret.tree->lson = tree->rson->lson;
        ret.tree->rson = tree->rson->rson;
        std::shared_ptr< Context< T > > cont(new Context< T >(tree->value, false));
        cont->context = context;
        cont->tree = tree->lson;
        ret.context = cont;
        return ret;    
      } else
        return *this;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::prune_right() const {
      if (has_left()) {
        BaseBTZipper ret(tree->lson->value);
        ret.context = context;
        ret.tree = tree->lson;
        return ret;
      } else
        return *this;
    }
    
    template < typename T >
    BaseBTZipper< T > BaseBTZipper< T >::prune_left() const {
      if (has_right()) {
        BaseBTZipper ret(tree->rson->value);
        ret.context = context;
        ret.tree = tree->rson;
        return ret;
      } else
        return *this;
    }
  
  } /* namespace implementation */
  
  template < typename T >
  class BTZipper : public implementation::BaseBTZipper< T > {
  public :
    BTZipper(const implementation::BaseBTZipper< T >& z) : implementation::BaseBTZipper< T >(z) {
    }
  };
  
} /* namespace btzipper */

/* --------------------------------------------------------*/ 
/* SECOND PART OF SOLUTION */
/* --------------------------------------------------------*/
namespace btzipper {
  
  template < typename Q > 
  class ExprNode {
  private :
    Q val;
    boost::function2< Q, Q, Q > func;
  public :
    enum NodeType {CONST, FUNC, VAR};
    NodeType type_;
      
    ExprNode(const Q& const_val) :  val(const_val), type_(CONST) {
    }
    ExprNode(const boost::function2< Q, Q, Q >& function) : func(function), type_(FUNC) {
    }
    ExprNode() : type_(VAR) {
    }
    NodeType type() {
      return type_;
    }
    Q value() {
      return val;
    }
    boost::function2< Q, Q, Q > function() {
      return func;
    }
  };
  
  namespace implementation {
    template < typename Q >
    Q compute(BTZipper< ExprNode< Q > > node, const Q& var) {
      switch (node.get().type()) {
        case ExprNode< Q >::NodeType::CONST :
          return node.get().value();
        break;
        case ExprNode< Q >::NodeType::FUNC :
          {
            Q larg(0), rarg(0);
            if (node.has_left())
              larg = compute< Q >(node.move_left(), var);
            if (node.has_right())
              rarg = compute< Q >(node.move_right(), var);
            return node.get().function()(larg, rarg);
          }
        break;
        case ExprNode< Q >::NodeType::VAR :
          return var;
        break;
      }
      throw std::exception();
    }
  } /* namespace implementation */
  
  template < typename Q >
  class BTZipper< ExprNode< Q > > : public implementation::BaseBTZipper< ExprNode< Q > > {
  public :
    BTZipper(const implementation::BaseBTZipper< ExprNode< Q > >& z) : 
      implementation::BaseBTZipper< ExprNode< Q > >(z) {
    }
    boost::function1< Q, Q > get_current_calculation() {
      return boost::bind(implementation::compute< Q >, *this, _1);
    }
    /* this part is not elegant, but deadlines are more important */
    BTZipper< ExprNode< Q > > move_up() const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::move_up();
    }
    BTZipper< ExprNode< Q > > move_left() const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::move_left();
    }
    BTZipper< ExprNode< Q > > move_right() const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::move_right();
    }
    BTZipper< ExprNode< Q > > prune_right() const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::prune_right();
    }
    BTZipper< ExprNode< Q > > prune_left() const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::prune_left();
    }
    BTZipper< ExprNode< Q > > push_left(ExprNode< Q > ex) const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::push_left(ex);
    }
    BTZipper< ExprNode< Q > > push_right(ExprNode< Q > ex) const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::push_right(ex);
    }
    BTZipper< ExprNode< Q > > set(ExprNode< Q > ex) const {
      return (BTZipper< ExprNode< Q > >)implementation::BaseBTZipper< ExprNode< Q > >::set(ex);
    }
  };
  
} /* namespace btzipper */

#endif /* __BTZIPPER_H */
