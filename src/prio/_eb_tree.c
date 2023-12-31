/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _eb_tree.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/impl/eb_tree.h>

// Stefan  Naeher ( 1989 )
// Michael Wenzel ( 1990 ) 


// ----------------------------------------------------------
//
// Konstruktor & Destruktor
//

l_stratified::l_stratified(stratified_ptr b, int x)
{
  int t = b->min2();

  if ( x == b->min() )
  {
    ma = t;
    mi = b->max();
  }

  if ( x == t )
  {
    ma = b->min();
    mi = b->max();
  }

  if ( x == b->max() )
  {
    ma = b->min();
    mi = t;
  }

}


stratified::stratified(int i)  
{ 

  mi = ma = -1;
  sz = 0; 
  k = i;
  top = 0;
  bot = 0;

}

stratified::stratified(l_stratified_ptr l, int i)
{ 

  mi = l->min();
  ma = l->max();
  sz = 2;
  k = i;
  top = 0;
  bot = 0;

}


stratified::~stratified()  
{ 
                                         // deallocate memory of sub-structures
  if ((k>1) && (sz>2))
  {                                      // delete stratified trees of bot
    bot->clear(k);

    if ( k <= 8 )
      delete bot->l;
    else
      delete bot->d;
    delete bot;

    if ( top->size() <= 2 )
      delete top;
    else
      delete (stratified_ptr)top;

    top = 0;
    bot = 0;

  }

}

// ----------------------------------------------------------
//
// min2                  
//
// liefert 2. kleinstes Element, falls sz > 2
//
// -1 , sonst
//
// ----------------------------------------------------------

int stratified::min2()

{
  if ( sz < 3 )
    return -1;

  int z = top->min();
  l_stratified_ptr l_bot_ptr = l_stratified_ptr(bot->lookup(z,k));

  return (mal_pot_2(z,down(k))) | (l_bot_ptr->min());

}


// ----------------------------------------------------------
//
// max2                  
//
// liefert 2. groesstes Element, falls sz > 2
//
// -1 , sonst
//
// ----------------------------------------------------------

int stratified::max2()

{
  if ( sz < 3 )
    return -1;

  int z = top->max();
  l_stratified_ptr l_bot_ptr = l_stratified_ptr(bot->lookup(z,k));

  return (mal_pot_2(z,down(k))) | (l_bot_ptr->max());

}


// ----------------------------------------------------------
//
// succ                  
//
// liefert Nachfolger von x im Baum auf dieser Rekursionsstufe
//                    falls existiert
// -1 , sonst
//
// ----------------------------------------------------------

int stratified::succ(int x) 

{
  if ( x >= ma ) return -1;
  if ( x <  mi ) return mi;
  if ( sz == 2 ) return ma;               // mi < x < ma

                                          // sz >= 2 &&  k>1
  int x1 = high_bits(x); 
  int x2 = low_bits(x); 

  GenPtr p = bot->lookup(x1,k);
  l_stratified_ptr l_bot_ptr = l_stratified_ptr(p);

  if ( (l_bot_ptr) && (l_bot_ptr->max()>x2) )
  { 
    if ( l_bot_ptr->size() <= 2 )             // l_stratified Struktur
        return ( mal_pot_2(x1,down(k))) | ( l_bot_ptr->succ(x2) ) ;
    else
    {
      stratified_ptr bot_ptr = stratified_ptr(p) ;
      return (mal_pot_2(x1,down(k))) | (bot_ptr->succ(x2)) ;
    }
  }
  else                                   // succ nicht in bot-Unterstruktur
  {
    int z;
    
    if ( top->size() <= 2 )
      z = top->succ(x1); 
    else
      z = ((stratified_ptr)top)->succ(x1); 

    if ( z == -1 ) 
      return ma;                         // x unmittelbar vor Maximum

    l_stratified_ptr l_bot_ptr = l_stratified_ptr(bot->lookup(z,k));

    return (mal_pot_2(z,down(k))) | (l_bot_ptr->min()) ;
  }

}

// ----------------------------------------------------------
//
// pred                  
//
// liefert Vorgaenger von x im Baum auf dieser Rekursionsstufe
//                    falls existiert
// -1 , sonst
//
// ----------------------------------------------------------

int stratified::pred(int x) 

{
  if ( x >  ma ) return ma;
  if ( x <= mi ) return -1;
  if ( sz == 2 ) return mi;               // mi < x < ma

					  // sz > 2 && k > 1
  int x1 = high_bits(x); 
  int x2 = low_bits(x); 

  GenPtr p = bot->lookup(x1,k);
  l_stratified_ptr l_bot_ptr = l_stratified_ptr(p);

  if ( (l_bot_ptr) && (l_bot_ptr->min()<x2) ) 
  {
    if ( l_bot_ptr->size() <= 2 )             // l_stratified Struktur
        return ( mal_pot_2(x1,down(k))) | ( l_bot_ptr->pred(x2) ) ;
    else
    {
      stratified_ptr bot_ptr = stratified_ptr(p) ;
      return (mal_pot_2(x1,down(k))) | (bot_ptr->pred(x2)) ;
    }
  }

  else                                        // pred nicht in top-Unterstruktur
    
  {
    int z;

    if ( top->size() <= 2 )
      z = top->pred(x1); 
    else
      z = ((stratified_ptr)top)->pred(x1); 

    if ( z == -1 ) 
      return mi;                              // x unmittelbar vor Maximum

    l_stratified_ptr l_bot_ptr = l_stratified_ptr(bot->lookup(z,k));

    return (mal_pot_2(z,down(k))) | (l_bot_ptr->max() ) ;
  }

} 

// --------------------------------------------------------------- 
//
// member   
//
// liefert 1, falls Element in der Struktur
//
//         0, sonst
//
// ----------------------------------------------------------

int stratified::member(int x)
{ 
  if (( x == ma )||( x == mi ))
    return 1;

  if (( x < mi )||( x > ma ))
    return 0;

  if ( sz == 2 )
    return 0;

				     // sz > 2 && k > 1
  int x1 = high_bits(x); 
  int x2 = low_bits(x); 

  GenPtr p = bot->lookup(x1,k);
  l_stratified_ptr l_bot_ptr = l_stratified_ptr(p);

  if (l_bot_ptr)

    if ( l_bot_ptr->size() <= 2 )    // l_stratified Struktur
      return l_bot_ptr->member(x2);

    else
    {
      stratified_ptr bot_ptr = stratified_ptr(p);
      return bot_ptr->member(x2);                  
    }

  else
    return 0;                

}

// ----------------------------------------------------------
//
// insert           
//
// fuegt ein Element x rekursiv in den Baum ein
//
// liefert 1, falls es eingefuegt wurde
//
//         0, falls es schon Element dewr Struktur war
//
// ----------------------------------------------------------

int stratified::insert(int x)
{ 
  int inserted = 0;

  if ( ( x == mi ) || ( x == ma ) )      // Element in Structure
    return 0;

  switch (sz)
  {

    case 0: 
	     {
	       mi = ma = x;

	       inserted = 1;
               break;
             }

    case 1:
	     {                           // incremental construction iff sz > 2
               if ( x > mi ) 
	         ma = x;
	       else
	         mi = x;

	       inserted = 1;
               break;
             }

    default: {                           // => k>1

	       if ( x > ma )
	       { 
		 int t = ma;
		 ma = x;
		 x = t;
               }
		 
               if ( x < mi )
	       { 
		 int t = mi;
		 mi = x;
		 x = t;
               }

               int x1 = high_bits(x);
               int x2 = low_bits(x);

                                         // for incremental construction

               if ( top == 0 )           // allocate new structures
               { 
	         top = new l_stratified(); 
                 bot = new b_dict(k);
               }          

               GenPtr& p = bot->lookup(x1,k);
               l_stratified_ptr l_bot_ptr = l_stratified_ptr(p);

	       if ( l_bot_ptr )
               {
                 if ( l_bot_ptr->size() == 1 )
		   inserted = l_bot_ptr->insert(x2);
		 else
                 {
		   stratified_ptr bot_ptr;

		   if ( l_bot_ptr->size() == 2 ) 
                   {
		     if ( !l_bot_ptr->member(x2) ) 
                     {
		       bot_ptr = new stratified(l_bot_ptr,down(k));
                       inserted = bot_ptr->insert(x2);

		       p = (GenPtr)bot_ptr;

		       delete l_bot_ptr;
                     }
                   }
		   else       // l_bot_ptr was a bot_ptr
                   {
                     bot_ptr = stratified_ptr(p) ;
                     inserted = bot_ptr->insert(x2);
                   }
                 }

               }
               else                // no top entry 
	       {
                 l_bot_ptr = new l_stratified(x2);

		 if ( top->size() <= 1 )
		   top->insert(x1);
                 else
                 { 
		   stratified_ptr ntop;

		   if ( top->size() == 2 )
		   { 
		     ntop = new stratified(top,up(k));
		     delete top;
		     top = (l_stratified_ptr)ntop;
                   }
		   else
		     ntop = (stratified_ptr)top;

	           ntop->insert(x1);
                 }

	         GenPtr help = GenPtr(l_bot_ptr);
	         bot->insert(x1,help,k);
		 inserted = 1;
               }
               break;
             }                           // default case

  }                                      // switch

  if ( inserted )
    sz++;

  return inserted;

}

// ----------------------------------------------------------
//
// del              
//
// streicht ein Element x rekursiv aus dem Baum 
//
// liefert 1, falls es gestrichen wurde
//
//         0, falls es nicht Element der Struktur war
//
// ----------------------------------------------------------

int stratified::del(int x)

{ 

  int deleted = 0;

  switch (sz)
  {

    case 0 : break;

    case 1 :
	     { 
               if ( x == mi )
	       { 
	         mi = ma = -1;
		 deleted = 1;
               }
	       break;
             }



    case 2 :
             {
               if ( x == mi ) 
               {
	         mi = ma;
	         deleted = 1;
	       }

               if ( x == ma )
               {
	         ma = mi;
	         deleted = 1;
	       }

	       break;
             }

    case 3 :       
                                   /* delete incremental construction */
             {           
               int t = min2();     /* third element */

               if ( x == mi )      /* new minimum   */
               {
	          mi = t;
		  deleted = 1;
               }

               if ( x == ma )      /* new maximum   */
               {
	          ma = t;
		  deleted = 1;
               }

	       if ( x == t )
		 deleted = 1;

               if ( deleted )
		            	   /* delete stratified trees of bot  */
               {
                 bot->clear(k);
		 if ( k <= 8 )
                   delete bot->l;
                 else
                   delete bot->d;
                 delete bot;
                 bot = 0;

                 delete top;
                 top = 0;
               }                   /* incremental construction deleted */

	       break;
             }         

    default :
             {
	       l_stratified_ptr l_bot_ptr;
	       stratified_ptr bot_ptr;

               if ( x == ma )                 /* delete maximum */
               {
		 int z = max2();
		 ma = z;
	         x = z;
               }

               if ( x == mi )                /* delete minimum */
               {
		 int z = min2();
		 mi = z;
	         x = z;
               }

               int x1 = high_bits(x);
               int x2 = low_bits(x);

               GenPtr& p = bot->lookup(x1,k);

	       if (!p)
		 return 0;                  /* element not in structure */

               l_bot_ptr = l_stratified_ptr(p) ;

               if ( l_bot_ptr->size() <= 2 )
	       {
                 deleted = l_bot_ptr->del(x2);

                 if ( l_bot_ptr->size() == 0 )  /* delete structure */
	         {
                   delete l_bot_ptr;
  
		   if ( top->size() <= 2 )
		     top->del(x1);
                   else
		   {
		     stratified_ptr ntop = (stratified_ptr)top;
		     if ( ntop->sz == 3 )
		     { 
		       top = new l_stratified(ntop,x1);
		       delete ntop;
                     }
		     else
                       ntop->del(x1);
                   } 

                   bot->del(x1,k);
                 }
               }

               else                                  /* stratified structure */
               {
                 bot_ptr = stratified_ptr(p) ;

                 if ( bot_ptr->sz == 3 )              /* change into l_stratified */
                 {
		   if ( bot_ptr->member(x2) )
		   {
		     l_bot_ptr = new l_stratified(bot_ptr,x2);
		     deleted = 1;

                     p = (GenPtr)l_bot_ptr;

                     delete bot_ptr;

                   }
                 }
                 else                               /* size >= 4 */
                   deleted = bot_ptr->del(x2);
               }

	       break;
             }

  }

  if ( deleted ) 
    sz--;

  return deleted;
}

// ----------------------------------------------------------
//
// print            
//
// druckt alle Elemente des Baumes aus            
//
// ----------------------------------------------------------

void stratified::print()
{ 
  int i = -1;
  while ((i=succ(i))>=0)
    cout << i << " ";
}

// -------------------------------------------------------
// clear
//
// loescht alle Elemente, zerstoert Unterbaeume und 
// setzt Dictionary auf Leerzustand
//
// ----------------------------------------------------------

b_dict::b_dict(int k) 
{ if ( k <= 8 ) 
   { l = new GenPtr[pot_2(up(k))];
     for (int i = 0; i < pot_2(up(k)); i++) l[i] = 0;
    }
  else 
   d = new dp_hash;
}

void b_dict::clear(int k)
{ 
  if ( k <= 8 )
  {
    for (int i = 0; i < pot_2(up(k)); i++)
    { 
      l_stratified_ptr l_bot_ptr=l_stratified_ptr(l[i]);
      if  (l_bot_ptr)
        if (l_bot_ptr->size()<=2) 
          delete l_bot_ptr;
        else
        { 
          stratified_ptr bot_ptr=stratified_ptr(l[i]);
          delete bot_ptr;
        }
    }

    return;
  }
      
		                         // b_dict was a hashing structure


  stp s = new subtable[d->n];
  stp pos = s;
  int i=0;

  while (i<d->sM)                        // Headertables loeschen
  {
    if (d->htablep[i])
    {
      d->htablep[i]->give_elements(pos,d->anf,d->ende);
      delete d->htablep[i];                         
    }
    i++;
  }

  for (i=0 ; i<d->n ; i++)
  { 
    l_stratified_ptr l_bot_ptr=l_stratified_ptr(s[i].info());
    if  (l_bot_ptr)
      if (l_bot_ptr->size()<=2) 
        delete l_bot_ptr;
      else
      { 
        stratified_ptr bot_ptr=stratified_ptr(s[i].info());
        delete bot_ptr;
      }
  }

  // free ((char*)d->htablep) ;             // Speicher freigeben

  delete[] d->htablep;

  if (d->anf)
    delete d->anf;

  delete s;

  d->n = 0;                              // Leerinitialisierung
  d->M=4;
  d->sM=13;
  d->k= 1 + (rand_int() & maxprim);
  d->bed=-17;
  d->htablep= new htp[d->sM];   //calloc(d->sM,sizeof(htp));
  d->anf = d->wo = d->ende = 0;

}
