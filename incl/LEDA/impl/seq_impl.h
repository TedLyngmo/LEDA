
typedef int seq_impl_item;

class seq_impl  {

virtual int  cmp(GenPtr, GenPtr) const = 0;
virtual int  int_type()          const = 0;
virtual void clear_key(GenPtr&)  const = 0;
virtual void clear_inf(GenPtr&)  const = 0;
virtual void copy_key(GenPtr&)   const = 0;
virtual void copy_inf(GenPtr&)   const = 0;


public:

 seq_impl();
 seq_impl(const seq_impl&);
~seq_impl();

seq_impl& operator=(const seq_impl&);
seq_impl& conc(seq_impl&);

seq_impl_item insert(GenPtr,GenPtr);
seq_impl_item insert_at_item(seq_impl_item,GenPtr,GenPtr);
seq_impl_item lookup(GenPtr)      const;
seq_impl_item locate(GenPtr)      const;
seq_impl_item locate_pred(GenPtr) const;
seq_impl_item succ(seq_impl_item) const;
seq_impl_item pred(seq_impl_item) const;
seq_impl_item item(GenPtr)        const;

GenPtr key(seq_impl_item) const;
GenPtr inf(seq_impl_item) const;

void reverse_items(seq_impl_item,seq_impl_item); 

void del(GenPtr); 
void del_item(seq_impl_item); 
void split_at_item(seq_impl_item,seq_impl&,seq_impl&);
void change_inf(seq_impl_item,GenPtr);
void clear();

int  size()  const;

};
