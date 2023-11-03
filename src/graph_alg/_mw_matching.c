/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _mw_matching.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


/* This file has been automatically generated from "max_weight_matching.w"
   by CTANGLE (Version 3.1 [km2c]) */

#include <LEDA/basic.h>
#include <LEDA/list.h>
#include <LEDA/ugraph.h>



typedef num_type GEWICHT_TYP;
typedef num_type DUALER_TYP;

typedef list <edge> KANTENLISTE;

#if !defined(__TEMPLATE_FUNCTIONS__)
LEDA_TYPE_PARAMETER(KANTENLISTE)
#endif

#define GENAUIGKEIT 1e-10

enum MARKENTYP {
  S = -1, UNMARKIERT, T
};

class blossom {
 public:

  list <list_item >ub_zeiger_liste;
  list <edge >kantenliste;


  node basis;
  DUALER_TYP z;


  list <node >knotenliste;


  MARKENTYP typ;
  edge kante;


  edge d3_kante;


  list <edge >adj_s_blossoms;



   blossom() {
    ub_zeiger_liste.clear();
    kantenliste.clear();
    knotenliste.clear();
    adj_s_blossoms.clear();
    basis = nil;
    z = 0;
    typ = UNMARKIERT;
    kante = nil;
    d3_kante = nil;
  }

   blossom(const list <list_item >&zliste, const list <edge >&eliste,
     const list <node >&nliste, const node b) {
    ub_zeiger_liste = zliste;
    kantenliste = eliste;
    knotenliste = nliste;
    adj_s_blossoms.clear();
    basis = b;

    z = 0;
    typ = S;
    kante = nil;
    d3_kante = nil;
  }

};

#if !defined(__TEMPLATE_FUNCTIONS__)
void Print(const blossom&, ostream&) {}
void Read(blossom&, istream&) {}
int compare(const blossom&, const blossom&) 
{ error_handler(1,"no compare for blossoms"); 
  return 0; 
}
LEDA_TYPE_PARAMETER(blossom)
#endif


int markiere
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <edge >&gematchte_kante,
   list <blossom > &blossomliste,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   list <edge >&pfad,
   node_array <edge >&d2_kante);

static void rek_erweitere
 (const ugraph &G,
   edge_array <bool >&gematcht,
   list <blossom > &unterblossomliste,
   blossom & bl,
   node j);

void erweitere
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <DUALER_TYP > &u,
   const list <edge >&pfad,
   edge_array <bool >&gematcht,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <edge >&gematchte_kante,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante);

int aendere_duale_variablen
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <edge >&gematchte_kante,
   list <blossom > &blossomliste,
   node_array <DUALER_TYP > &u,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   const node_array <edge >&d2_kante);

void bilde_blossom
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   const list <edge >&pfad,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <list_item >&blossom_zu_knoten,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante);

void expandiere_blossoms
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <list_item >&blossom_zu_knoten,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante);

void neues_s_blossom
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   list <blossom > &blossomliste,
   const node_array <list_item >&blossom_zu_knoten,
   const edge_array <bool >&gematcht,
   blossom & bl,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante);

static void aktualisiere_knotenliste
 (list <blossom > &unterblossomliste,
   blossom & bl);


static void matchingtest
 (const ugraph &G,
   const edge_array <bool >&gematcht);

static void bed1test
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const list <blossom > &blossomliste,
   const list <blossom > &unterblossomliste,
   const node_array <list_item >&blossom_zu_knoten);

static void bed2test
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const list <blossom > &blossomliste,
   const list <blossom > &unterblossomliste,
   const node_array <list_item >&blossom_zu_knoten,
   const edge_array <bool >&gematcht,
   const node_array <DUALER_TYP > &u);

static void bed3test
 (const ugraph &G,
   const node_array <DUALER_TYP > &u,
   const node_array <edge >&gematchte_kante);

static void bed4test
 (const ugraph &G,
   const list <blossom > &blossomliste,
   const list <blossom > &unterblossomliste,
   const node_array <edge >&gematchte_kante,
   const node_array <list_item >&blossom_zu_knoten);

static DUALER_TYP bestimme_z
 (const ugraph &G,
   const list <blossom > &blossomliste,
   const list <blossom > &unterblossomliste,
   const blossom & bl,
   const edge &e);



list <edge >MAX_WEIGHT_MATCHING (const ugraph &G, 
                                 const edge_array <GEWICHT_TYP > &gewicht) 
{

  edge_array <bool >gematcht;
  gematcht.init(G, false);


  list <blossom > blossomliste;
  list <blossom > unterblossomliste;

  node i;

  forall_nodes (i, G) {
    list <list_item >l1;
    list <edge >l2;
    list <node >l3;

    blossomliste.append(blossom(l1, l2, l3, i));
  }


  node_array <list_item >blossom_zu_knoten(G);

  if (!blossomliste.empty()) {
    list_item lit = blossomliste.first();

    while (lit != nil) {
      node i = blossomliste[lit].basis;
      blossom_zu_knoten[i] = lit;

      lit = blossomliste.succ(lit);
    }
  }


  node_array <DUALER_TYP > u(G);
  GEWICHT_TYP w_max = 0;
  edge e;

  forall_edges (e, G) {
    if (gewicht[e] > w_max)
      w_max = gewicht[e];
  }

  u.init(G, num_type(w_max / 2.0));


  node_array <edge >gematchte_kante(G, nil);


  list <edge >l;
  node_array <KANTENLISTE > brauchbare_kanten(G, l);


  list <node >ungescannt;


  node_array <bool >schon_eingefuegt(G, false);
  forall_edges (e, G) {
    if ((gewicht[e] == w_max) ||
      ((gewicht[e] < w_max) && (w_max - gewicht[e] < GENAUIGKEIT)) ||
      ((gewicht[e] > w_max) && (gewicht[e] - w_max < GENAUIGKEIT))) {
      if (schon_eingefuegt[G.source(e)] == false) {
	ungescannt.append(G.source(e));
	schon_eingefuegt[G.source(e)] = true;
      }

      if (schon_eingefuegt[G.target(e)] == false) {
	ungescannt.append(G.target(e));
	schon_eingefuegt[G.target(e)] = true;
      }

      brauchbare_kanten[G.source(e)].append(e);
      brauchbare_kanten[G.target(e)].append(e);
    }
  }


  list <edge >pfad;


  node_array <edge >d2_kante(G, nil);



  bool maximal = false;

  while (!maximal) {
    pfad.clear();


    int erg_markiere = markiere
    (G, gewicht, u, gematcht, blossom_zu_knoten, gematchte_kante,
      blossomliste, ungescannt, brauchbare_kanten, pfad, d2_kante);


    if (erg_markiere > 0) {


      erweitere
	(G, gewicht, blossom_zu_knoten, u, pfad, gematcht, blossomliste,
	unterblossomliste, ungescannt, gematchte_kante, brauchbare_kanten, d2_kante);


    }

    else if (erg_markiere < 0) {


      bilde_blossom
	(G, gewicht, u, gematcht, pfad, blossomliste, unterblossomliste, ungescannt,
	blossom_zu_knoten, brauchbare_kanten, d2_kante);


    }

    else if (erg_markiere == 0) {


      int erg_aendere = aendere_duale_variablen
      (G, gewicht, blossom_zu_knoten, gematchte_kante,
	blossomliste, u, ungescannt, brauchbare_kanten, d2_kante);


      if (erg_aendere > 0)
	maximal = true;

      else if (erg_aendere < 0) {


	expandiere_blossoms
	  (G, gewicht, u, gematcht, blossomliste, unterblossomliste, ungescannt,
	  blossom_zu_knoten, brauchbare_kanten, d2_kante);


      }
    }
  }


  matchingtest(G, gematcht);
  bed1test(G, gewicht, u, blossomliste, unterblossomliste, blossom_zu_knoten);
  bed2test(G, gewicht, blossomliste, unterblossomliste, blossom_zu_knoten, gematcht, u);
  bed3test(G, u, gematchte_kante);
  bed4test(G, blossomliste, unterblossomliste, gematchte_kante, blossom_zu_knoten);


  list <edge >matching;

  forall_edges (e, G)
    if (gematcht[e] == true) {
      matching.append(e);
    }

  return matching;
}



int markiere
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <edge >&gematchte_kante,
   list <blossom > &blossomliste,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   list <edge >&pfad,
   node_array <edge >&d2_kante) {

  if (!ungescannt.empty()) {
    list_item lit_ungescannt = ungescannt.first();

    while (lit_ungescannt != nil) {
      node i = ungescannt.pop();


      while (!brauchbare_kanten[i].empty()) {
	edge e = brauchbare_kanten[i].pop();
	node j = G.opposite(i, e);

	if (blossomliste[blossom_zu_knoten[j]].typ == UNMARKIERT) {

	  blossomliste[blossom_zu_knoten[j]].typ = T;
	  blossomliste[blossom_zu_knoten[j]].kante = e;


	  node b = blossomliste[blossom_zu_knoten[j]].basis;
	  edge e_gematcht = gematchte_kante[b];

	  list_item bllit = blossom_zu_knoten[G.opposite(b, e_gematcht)];
	  blossomliste[bllit].typ = S;
	  blossomliste[bllit].kante = e_gematcht;

	  neues_s_blossom
	    (G, gewicht, u, blossomliste, blossom_zu_knoten, gematcht, blossomliste[bllit],
	    ungescannt, brauchbare_kanten, d2_kante);


	}

	else if ((blossomliste[blossom_zu_knoten[j]].typ == S) &&
	  (blossom_zu_knoten[i] != blossom_zu_knoten[j])) {

	  ungescannt.push(i);

	  node start[2];
	  start[0] = i;
	  start[1] = j;

	  list <edge >alt_pfad[2];

	  for (int index = 0; index < 2; index++) {

	    edge kante = blossomliste[blossom_zu_knoten[start[index]]].kante;

	    while (kante != nil) {
	      node s = G.source(kante);
	      node t = G.target(kante);

	      alt_pfad[index].push(kante);

	      if (blossom_zu_knoten[s] == blossom_zu_knoten[start[index]]) {
		start[index] = t;
	      }

	      else {
		start[index] = s;
	      }

	      kante = blossomliste[blossom_zu_knoten[start[index]]].kante;
	    }

	    start[index] = blossomliste[blossom_zu_knoten[start[index]]].basis;


	  }


	  if (start[0] == start[1]) {

	    while (alt_pfad[0].head() == alt_pfad[1].head()) {
	      alt_pfad[0].pop();
	      alt_pfad[1].pop();
	    }



	    pfad = alt_pfad[0];
	    pfad.append(e);

	    if (!alt_pfad[1].empty()) {
	      list_item lit = alt_pfad[1].last();

	      while (lit != nil) {
		pfad.append(alt_pfad[1].inf(lit));
		lit = alt_pfad[1].pred(lit);
	      }
	    }

	    return -1;
	  }

	  else {

	    pfad = alt_pfad[0];
	    pfad.append(e);

	    if (!alt_pfad[1].empty()) {
	      list_item lit = alt_pfad[1].last();

	      while (lit != nil) {
		pfad.append(alt_pfad[1].inf(lit));
		lit = alt_pfad[1].pred(lit);
	      }
	    }

	    return 1;
	  }


	}

	else if (blossomliste[blossom_zu_knoten[j]].typ == T) {
	  brauchbare_kanten[j].append(e);
	}
      }


      lit_ungescannt = ungescannt.first();
    }
  }

 return 0;


}



void neues_s_blossom
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   list <blossom > &blossomliste,
   const node_array <list_item >&blossom_zu_knoten,
   const edge_array <bool >&gematcht,
   blossom & bl,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante) {
  if (!bl.knotenliste.empty()) {
    list_item knotenlit = bl.knotenliste.first();

    while (knotenlit != nil) {
      node i = bl.knotenliste.inf(knotenlit);


      edge e;
      bool knoten_brauchbar = false;

      brauchbare_kanten[i].clear();
      d2_kante[i] = nil;

      forall_adj_edges (e, i) {
	if (gematcht[e] == false) {
	  list_item litsource = blossom_zu_knoten[G.source(e)];
	  list_item littarget = blossom_zu_knoten[G.target(e)];

	  if (litsource != littarget) {
	    DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	    if ((pi == 0) ||
	      (pi < 0 && (-pi) < GENAUIGKEIT) ||
	      (pi > 0 && pi < GENAUIGKEIT)) {
	      brauchbare_kanten[i].append(e);
	      knoten_brauchbar = true;
	    }

	    else {

	      list_item bllit;
	      node j;

	      if (G.source(e) == i) {
		bllit = littarget;
		j = G.target(e);
	      }

	      else {
		bllit = litsource;
		j = G.source(e);
	      }


	      if ((blossomliste[bllit].typ == UNMARKIERT) ||
		(blossomliste[bllit].typ == T)) {
		if (d2_kante[j] != nil) {
		  edge min_e = d2_kante[j];
		  DUALER_TYP pi_j;
		  pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		  if (pi < pi_j)
		    d2_kante[j] = e;
		}

		else
		  d2_kante[j] = e;
	      }


	      else if (blossomliste[bllit].typ == S) {
		if ((pi != 0) ||
		  (pi < 0 && (-pi) < GENAUIGKEIT) ||
		  (pi > 0 && pi < GENAUIGKEIT)) {
		  blossomliste[litsource].adj_s_blossoms.append(e);
		  blossomliste[littarget].adj_s_blossoms.append(e);
		}

		if (bl.d3_kante != nil) {
		  edge min_e = bl.d3_kante;
		  DUALER_TYP pi_j;
		  pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		  if (pi < pi_j)
		    bl.d3_kante = e;
		}

		else
		  bl.d3_kante = e;

		if (blossomliste[bllit].d3_kante != nil) {
		  edge min_e = blossomliste[bllit].d3_kante;
		  DUALER_TYP pi_j;
		  pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		  if (pi < pi_j)
		    blossomliste[bllit].d3_kante = e;
		}

		else
		  blossomliste[bllit].d3_kante = e;
	      }
	    }
	  }
	}
      }

      if (knoten_brauchbar == true)
	ungescannt.push(i);



      knotenlit = bl.knotenliste.succ(knotenlit);
    }
  }

  else {
    node i = bl.basis;

    edge e;
    bool knoten_brauchbar = false;

    brauchbare_kanten[i].clear();
    d2_kante[i] = nil;

    forall_adj_edges (e, i) {
      if (gematcht[e] == false) {
	list_item litsource = blossom_zu_knoten[G.source(e)];
	list_item littarget = blossom_zu_knoten[G.target(e)];

	if (litsource != littarget) {
	  DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	  if ((pi == 0) ||
	    (pi < 0 && (-pi) < GENAUIGKEIT) ||
	    (pi > 0 && pi < GENAUIGKEIT)) {
	    brauchbare_kanten[i].append(e);
	    knoten_brauchbar = true;
	  }

	  else {

	    list_item bllit;
	    node j;

	    if (G.source(e) == i) {
	      bllit = littarget;
	      j = G.target(e);
	    }

	    else {
	      bllit = litsource;
	      j = G.source(e);
	    }


	    if ((blossomliste[bllit].typ == UNMARKIERT) ||
	      (blossomliste[bllit].typ == T)) {
	      if (d2_kante[j] != nil) {
		edge min_e = d2_kante[j];
		DUALER_TYP pi_j;
		pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		if (pi < pi_j)
		  d2_kante[j] = e;
	      }

	      else
		d2_kante[j] = e;
	    }


	    else if (blossomliste[bllit].typ == S) {
	      if ((pi != 0) ||
		(pi < 0 && (-pi) < GENAUIGKEIT) ||
		(pi > 0 && pi < GENAUIGKEIT)) {
		blossomliste[litsource].adj_s_blossoms.append(e);
		blossomliste[littarget].adj_s_blossoms.append(e);
	      }

	      if (bl.d3_kante != nil) {
		edge min_e = bl.d3_kante;
		DUALER_TYP pi_j;
		pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		if (pi < pi_j)
		  bl.d3_kante = e;
	      }

	      else
		bl.d3_kante = e;

	      if (blossomliste[bllit].d3_kante != nil) {
		edge min_e = blossomliste[bllit].d3_kante;
		DUALER_TYP pi_j;
		pi_j = u[G.source(min_e)] + u[G.target(min_e)] - gewicht[min_e];

		if (pi < pi_j)
		  blossomliste[bllit].d3_kante = e;
	      }

	      else
		blossomliste[bllit].d3_kante = e;
	    }
	  }
	}
      }
    }

    if (knoten_brauchbar == true)
      ungescannt.push(i);


  }
}




void erweitere
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <DUALER_TYP > &u,
   const list <edge >&pfad,
   edge_array <bool >&gematcht,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <edge >&gematchte_kante,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante) {

  if (!pfad.empty()) {
    list_item lit = pfad.first();

    while (lit != nil) {
      edge e = pfad.inf(lit);

      if (gematcht[e] == true) {
	gematcht[e] = false;
      }

      else {
	gematcht[e] = true;
	node s = G.source(e);
	node t = G.target(e);

	if (s != blossomliste[blossom_zu_knoten[s]].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, blossomliste[blossom_zu_knoten[s]], s);

	  aktualisiere_knotenliste
	    (unterblossomliste, blossomliste[blossom_zu_knoten[s]]);
	}

	if (t != blossomliste[blossom_zu_knoten[t]].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, blossomliste[blossom_zu_knoten[t]], t);

	  aktualisiere_knotenliste
	    (unterblossomliste, blossomliste[blossom_zu_knoten[t]]);
	}


      }

      lit = pfad.succ(lit);
    }
  }



  gematchte_kante.init(G, nil);

  edge e;

  forall_edges (e, G) {
    if (gematcht[e] == true) {
      gematchte_kante[G.source(e)] = e;
      gematchte_kante[G.target(e)] = e;
    }
  }



  list <edge >l;
  brauchbare_kanten.init(G, l);
  ungescannt.clear();

  d2_kante.init(G, nil);

  if (!blossomliste.empty()) {
    list_item bllit = blossomliste.first();

    while (bllit != nil) {
      blossomliste[bllit].typ = UNMARKIERT;
      blossomliste[bllit].d3_kante = nil;
      blossomliste[bllit].kante = nil;
      blossomliste[bllit].adj_s_blossoms.clear();

      bllit = blossomliste.succ(bllit);
    }
  }


  if (!blossomliste.empty()) {
    list_item bllit = blossomliste.first();

    while (bllit != nil) {
      if (gematchte_kante[blossomliste[bllit].basis] == nil) {
	blossomliste[bllit].typ = S;

	neues_s_blossom
	  (G, gewicht, u, blossomliste, blossom_zu_knoten, gematcht,
	  blossomliste[bllit], ungescannt, brauchbare_kanten, d2_kante);
      }

      bllit = blossomliste.succ(bllit);
    }
  }
}



static void rek_erweitere
 (const ugraph &G,
   edge_array <bool >&gematcht,
   list <blossom > &unterblossomliste,
   blossom & bl,
   node j) {

  list_item knotenlit = bl.knotenliste.first();
  node i = bl.knotenliste.inf(knotenlit);


  list_item ubzlit = bl.ub_zeiger_liste.first();
  list_item ubllit = bl.ub_zeiger_liste[ubzlit];
  list_item kantenlit = bl.kantenliste.first();


  while (i != j) {
    node b = unterblossomliste[ubllit].basis;


    if (i == b) {
      ubzlit = bl.ub_zeiger_liste.succ(ubzlit);
      ubllit = bl.ub_zeiger_liste[ubzlit];
      kantenlit = bl.kantenliste.succ(kantenlit);
    }



    knotenlit = bl.knotenliste.succ(knotenlit);
    i = bl.knotenliste.inf(knotenlit);
  }



  list <edge >pfad;
  list <list_item >ub_liste;
  list <node >exitknotenliste;



  list_item hklit = kantenlit;
  list_item hzlit = ubzlit;

  if (gematcht[bl.kantenliste.inf(kantenlit)] == true) {
    while (kantenlit != nil) {
      pfad.append(bl.kantenliste.inf(kantenlit));
      kantenlit = bl.kantenliste.pred(kantenlit);

      ub_liste.append(bl.ub_zeiger_liste[ubzlit]);
      ubzlit = bl.ub_zeiger_liste.pred(ubzlit);
    }

    ubzlit = bl.ub_zeiger_liste.last();
    ub_liste.append(bl.ub_zeiger_liste[ubzlit]);
  }


  else {
    kantenlit = bl.kantenliste.succ(kantenlit);

    while (kantenlit != nil) {
      pfad.append(bl.kantenliste.inf(kantenlit));

      kantenlit = bl.kantenliste.succ(kantenlit);

      ub_liste.append(bl.ub_zeiger_liste[ubzlit]);
      ubzlit = bl.ub_zeiger_liste.succ(ubzlit);
    }

    ub_liste.append(bl.ub_zeiger_liste[ubzlit]);
  }



  kantenlit = pfad.first();

  while (kantenlit != nil) {
    edge e = pfad.inf(kantenlit);

    if (gematcht[e] == true) {
      gematcht[e] = false;
    }

    else {
      gematcht[e] = true;
      exitknotenliste.append(G.source(e));
      exitknotenliste.append(G.target(e));
    }

    kantenlit = pfad.succ(kantenlit);
  }



  list <list_item >ubzliste[2];

  ubzlit = hzlit;
  kantenlit = hklit;

  ubzlit = bl.ub_zeiger_liste.succ(ubzlit);

  bl.ub_zeiger_liste.split(ubzlit, ubzliste[0], ubzliste[1]);
  bl.ub_zeiger_liste.conc(ubzliste[1]);
  bl.ub_zeiger_liste.conc(ubzliste[0]);


  list <edge >kaliste[2];

  kantenlit = bl.kantenliste.succ(kantenlit);

  bl.kantenliste.split(kantenlit, kaliste[0], kaliste[1]);
  bl.kantenliste.conc(kaliste[1]);
  bl.kantenliste.conc(kaliste[0]);



  bl.basis = j;



  list_item lit = ub_liste.pop();
  node b = unterblossomliste[lit].basis;

  if (j != b) {
    rek_erweitere
      (G, gematcht, unterblossomliste, unterblossomliste[lit], j);

    aktualisiere_knotenliste
      (unterblossomliste, unterblossomliste[lit]);
  }


  while (!exitknotenliste.empty()) {
    node k = exitknotenliste.pop();
    node l = exitknotenliste.pop();

    list_item lit = ub_liste.pop();
    list_item lit2 = ub_liste.pop();


    if ((!unterblossomliste[lit].knotenliste.empty()) &&
      (!unterblossomliste[lit2].knotenliste.empty())) {
      list_item knotenlit = unterblossomliste[lit].knotenliste.first();

      while (knotenlit != nil) {
	node hilfsknoten = unterblossomliste[lit].knotenliste[knotenlit];


	if (hilfsknoten == k) {
	  if (k != unterblossomliste[lit].basis) {
	    rek_erweitere
	      (G, gematcht, unterblossomliste, unterblossomliste[lit], k);

	    aktualisiere_knotenliste
	      (unterblossomliste, unterblossomliste[lit]);
	  }

	  if (l != unterblossomliste[lit2].basis) {
	    rek_erweitere
	      (G, gematcht, unterblossomliste, unterblossomliste[lit2], l);

	    aktualisiere_knotenliste
	      (unterblossomliste, unterblossomliste[lit2]);
	  }

	  break;
	}



	else if (hilfsknoten == l) {
	  if (l != unterblossomliste[lit].basis) {
	    rek_erweitere
	      (G, gematcht, unterblossomliste, unterblossomliste[lit], l);

	    aktualisiere_knotenliste
	      (unterblossomliste, unterblossomliste[lit]);
	  }

	  if (k != unterblossomliste[lit2].basis) {
	    rek_erweitere
	      (G, gematcht, unterblossomliste, unterblossomliste[lit2], k);

	    aktualisiere_knotenliste
	      (unterblossomliste, unterblossomliste[lit2]);
	  }

	  break;
	}



	knotenlit = unterblossomliste[lit].knotenliste.succ(knotenlit);
      }
    }


    else if ((unterblossomliste[lit].knotenliste.empty()) &&
      (!unterblossomliste[lit2].knotenliste.empty())) {
      if (k == unterblossomliste[lit].basis) {
	if (l != unterblossomliste[lit2].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, unterblossomliste[lit2], l);

	  aktualisiere_knotenliste
	    (unterblossomliste, unterblossomliste[lit2]);
	}
      }

      else {
	if (k != unterblossomliste[lit2].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, unterblossomliste[lit2], k);

	  aktualisiere_knotenliste
	    (unterblossomliste, unterblossomliste[lit2]);
	}
      }
    }


    else if ((!unterblossomliste[lit].knotenliste.empty()) &&
      (unterblossomliste[lit2].knotenliste.empty())) {
      if (k == unterblossomliste[lit2].basis) {
	if (l != unterblossomliste[lit].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, unterblossomliste[lit], l);

	  aktualisiere_knotenliste
	    (unterblossomliste, unterblossomliste[lit]);
	}
      }

      else {
	if (k != unterblossomliste[lit].basis) {
	  rek_erweitere
	    (G, gematcht, unterblossomliste, unterblossomliste[lit], k);

	  aktualisiere_knotenliste
	    (unterblossomliste, unterblossomliste[lit]);
	}
      }
    }
  }


}



static void aktualisiere_knotenliste
 (list <blossom > &unterblossomliste, blossom & bl) {
  bl.knotenliste.clear();

  if (!bl.ub_zeiger_liste.empty()) {
    list_item ubzlit = bl.ub_zeiger_liste.first();

    while (ubzlit != nil) {
      list_item ubllit = bl.ub_zeiger_liste[ubzlit];

      if (!unterblossomliste[ubllit].knotenliste.empty()) {
	list_item knotenlit = unterblossomliste[ubllit].knotenliste.first();

	while (knotenlit != nil) {
	  node i = unterblossomliste[ubllit].knotenliste.inf(knotenlit);
	  bl.knotenliste.append(i);

	  knotenlit = unterblossomliste[ubllit].knotenliste.succ(knotenlit);
	}
      }

      else {
	bl.knotenliste.append(unterblossomliste[ubllit].basis);
      }

      ubzlit = bl.ub_zeiger_liste.succ(ubzlit);
    }
  }
}




void bilde_blossom
 (const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   const list <edge >&pfad,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <list_item >&blossom_zu_knoten,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante) {

  node neue_basis;
  list_item neues_blossom;
  list <list_item >zliste;
  list <node >nliste;
  edge hilfskante;



  edge e1 = pfad.inf(pfad.first());
  edge e2 = pfad.inf(pfad.last());

  node i1 = G.source(e1);
  node i2 = G.target(e1);
  node i3 = G.source(e2);
  node i4 = G.target(e2);

  if ((blossom_zu_knoten[i1] == blossom_zu_knoten[i3]) ||
    (blossom_zu_knoten[i1] == blossom_zu_knoten[i4])) {
    neue_basis = blossomliste[blossom_zu_knoten[i1]].basis;
  }

  else
    neue_basis = blossomliste[blossom_zu_knoten[i2]].basis;



  list_item litpfad = pfad.first();

  while (pfad.succ(litpfad) != nil) {

    list_item bllit;
    edge e1 = pfad.inf(litpfad);
    edge e2 = pfad.inf(pfad.succ(litpfad));

    node i1 = G.source(e1);
    node i2 = G.target(e1);
    node i3 = G.source(e2);
    node i4 = G.target(e2);

    if ((blossom_zu_knoten[i1] == blossom_zu_knoten[i3]) ||
      (blossom_zu_knoten[i1] == blossom_zu_knoten[i4])) {
      bllit = blossom_zu_knoten[i1];
    }

    else
      bllit = blossom_zu_knoten[i2];



    if (!blossomliste[bllit].knotenliste.empty()) {
      list_item knotenlit = blossomliste[bllit].knotenliste.first();

      while (knotenlit != nil) {
	node i = blossomliste[bllit].knotenliste[knotenlit];
	nliste.append(i);

	knotenlit = blossomliste[bllit].knotenliste.succ(knotenlit);
      }
    }

    else {
      node i = blossomliste[bllit].basis;

      nliste.append(i);
    }


    if (blossomliste[bllit].typ == T) {
      neues_s_blossom(G, gewicht, u, blossomliste, blossom_zu_knoten, gematcht,
	blossomliste[bllit], ungescannt, brauchbare_kanten, d2_kante);
    }


    blossom bl = blossomliste.del_item(bllit);
    bllit = unterblossomliste.append(bl);
    unterblossomliste[bllit].typ = UNMARKIERT;
    unterblossomliste[bllit].kante = nil;

    zliste.append(bllit);



    litpfad = pfad.succ(litpfad);
  }


  list_item bllit = blossom_zu_knoten[neue_basis];
  blossom bl = blossomliste.del_item(bllit);
  hilfskante = bl.kante;
  bllit = unterblossomliste.append(bl);
  unterblossomliste[bllit].typ = UNMARKIERT;
  unterblossomliste[bllit].kante = nil;

  zliste.append(bllit);

  if (!bl.knotenliste.empty()) {
    list_item lit = bl.knotenliste.first();

    while (lit != nil) {
      nliste.append(bl.knotenliste.inf(lit));
      lit = bl.knotenliste.succ(lit);
    }
  }

  else
    nliste.append(bl.basis);





  bl = blossom(zliste, pfad, nliste, neue_basis);
  bl.typ = S;
  bl.kante = hilfskante;
  neues_blossom = blossomliste.append(bl);



  list_item litn = nliste.first();

  while (litn != nil) {
    node i = nliste[litn];
    blossom_zu_knoten[i] = neues_blossom;

    litn = nliste.succ(litn);
  }



  if (!blossomliste[neues_blossom].ub_zeiger_liste.empty()) {
    list_item ubzlit = blossomliste[neues_blossom].ub_zeiger_liste.first();

    while (ubzlit != nil) {
      list_item ubllit = blossomliste[neues_blossom].ub_zeiger_liste[ubzlit];

      if (unterblossomliste[ubllit].d3_kante != nil) {
	edge e = unterblossomliste[ubllit].d3_kante;
	node i = G.source(e);
	node j = G.target(e);
	DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	if ((blossom_zu_knoten[i] == blossom_zu_knoten[j]) ||
	  (pi == 0) || (pi < 0 && (-pi) < GENAUIGKEIT) || (pi > 0 && pi < GENAUIGKEIT)) {
	  unterblossomliste[ubllit].d3_kante = nil;


	  list <list_item >loesche_kante;

	  if (!unterblossomliste[ubllit].adj_s_blossoms.empty()) {
	    list_item kantenlit = unterblossomliste[ubllit].adj_s_blossoms.first();

	    while (kantenlit != nil) {
	      edge e = unterblossomliste[ubllit].adj_s_blossoms[kantenlit];

	      DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	      if ((blossom_zu_knoten[G.source(e)] != blossom_zu_knoten[G.target(e)]) &&
		(pi > GENAUIGKEIT || pi < -GENAUIGKEIT)) {
		if (unterblossomliste[ubllit].d3_kante == nil)
		  unterblossomliste[ubllit].d3_kante = e;

		else {
		  edge e_min = unterblossomliste[ubllit].d3_kante;

		  DUALER_TYP pi_min = u[G.source(e_min)] + u[G.target(e_min)] - gewicht[e_min];

		  if (pi < pi_min)
		    unterblossomliste[ubllit].d3_kante = e;
		}
	      }

	      else
		loesche_kante.append(kantenlit);

	      kantenlit = unterblossomliste[ubllit].adj_s_blossoms.succ(kantenlit);
	    }
	  }


	  while (!loesche_kante.empty()) {
	    list_item lit = loesche_kante.pop();
	    blossomliste[ubllit].adj_s_blossoms.del_item(lit);
	  }


	}
      }

      ubzlit = blossomliste[neues_blossom].ub_zeiger_liste.succ(ubzlit);
    }
  }


  if (!blossomliste[neues_blossom].ub_zeiger_liste.empty()) {
    list_item ubzlit = blossomliste[neues_blossom].ub_zeiger_liste.first();

    while (ubzlit != nil) {
      list_item ubllit = blossomliste[neues_blossom].ub_zeiger_liste[ubzlit];

      if (unterblossomliste[ubllit].d3_kante != nil) {

	edge e = unterblossomliste[ubllit].d3_kante;

	DUALER_TYP pi_e = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	if (blossomliste[neues_blossom].d3_kante == nil)
	  blossomliste[neues_blossom].d3_kante = e;

	else {
	  edge e_min = blossomliste[neues_blossom].d3_kante;
	  DUALER_TYP pi_min;
	  pi_min = u[G.source(e_min)] + u[G.target(e_min)] - gewicht[e_min];

	  if (pi_e < pi_min)
	    blossomliste[neues_blossom].d3_kante = e;
	}

	blossomliste[neues_blossom].adj_s_blossoms.append(e);

	unterblossomliste[ubllit].d3_kante = nil;
      }

      unterblossomliste[ubllit].adj_s_blossoms.clear();

      ubzlit = blossomliste[neues_blossom].ub_zeiger_liste.succ(ubzlit);
    }
  }


}



int aendere_duale_variablen
 (const ugraph &G, const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <list_item >&blossom_zu_knoten,
   const node_array <edge >&gematchte_kante,
   list <blossom > &blossomliste,
   node_array <DUALER_TYP > &u,
   list <node >&ungescannt,
   node_array <KANTENLISTE > &brauchbare_kanten,
   const node_array <edge >&d2_kante) {

  DUALER_TYP delta, d_1, d_2, d_3, d_4;
  int erg_aendere;


  node_array <bool >schon_eingefuegt;
  schon_eingefuegt.init(G, false);



  d_1 = 0;
  d_2 = 0;

  node i;
  forall_nodes (i, G) {
    if (gematchte_kante[i] == nil) {
      d_1 = u[i];
    }

    list_item bllit = blossom_zu_knoten[i];

    if ((blossomliste[bllit].typ == UNMARKIERT) && (d2_kante[i] != nil)) {
      edge e = d2_kante[i];

      DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

      if ((d_2 == 0) || (pi < d_2)) {
	d_2 = pi;
      }
    }
  }

  if (d_2 == 0)
    d_2 = d_1;


  d_3 = d_1;
  d_4 = 2 * d_1;

  if (!blossomliste.empty()) {
    list_item bllit = blossomliste.first();

    while (bllit != nil) {
      if ((blossomliste[bllit].typ == S) && (blossomliste[bllit].d3_kante != nil)) {
	edge e = blossomliste[bllit].d3_kante;

	DUALER_TYP pi = (u[G.source(e)] + u[G.target(e)] - gewicht[e]) / 2;
	if (pi < d_3)
	  d_3 = pi;
      }

      else if ((!blossomliste[bllit].knotenliste.empty()) &&
	(blossomliste[bllit].typ == T)) {
	if (blossomliste[bllit].z < d_4) {
	  d_4 = blossomliste[bllit].z;
	}
      }

      bllit = blossomliste.succ(bllit);
    }
  }

  d_4 = d_4 / 2;


  if ((d_1 <= d_2) && (d_1 <= d_3) && (d_1 <= d_4)) {
    delta = d_1;
    erg_aendere = 1;
  }

  else if ((d_2 <= d_3) && (d_2 <= d_4)) {
    delta = d_2;
    erg_aendere = 0;
  }

  else if (d_3 <= d_4) {
    delta = d_3;
    erg_aendere = 0;
  }

  else {
    delta = d_4;
    erg_aendere = -1;
  }



  forall_nodes (i, G) {
    if (blossomliste[blossom_zu_knoten[i]].typ == S) {
      u[i] = u[i] - delta;
    }

    if (blossomliste[blossom_zu_knoten[i]].typ == T) {
      u[i] = u[i] + delta;
    }
  }


  if (!blossomliste.empty()) {
    list_item bllit = blossomliste.first();

    while (bllit != nil) {
      if (!blossomliste[bllit].knotenliste.empty()) {
	if (blossomliste[bllit].typ == S) {
	  blossomliste[bllit].z = blossomliste[bllit].z + 2 * delta;
	}

	else if (blossomliste[bllit].typ == T) {
	  blossomliste[bllit].z = blossomliste[bllit].z - 2 * delta;
	}
      }

      bllit = blossomliste.succ(bllit);
    }
  }



  if (delta == d_1)
    return erg_aendere;
  forall_nodes (i, G) {
    if ((blossomliste[blossom_zu_knoten[i]].typ == UNMARKIERT) &&
      (d2_kante[i] != nil)) {
      edge e = d2_kante[i];
      node j;

      if (i == G.target(e))
	j = G.source(e);
      else
	j = G.target(e);

      DUALER_TYP pi = u[i] + u[j] - gewicht[e];

      if ((pi == 0) ||
	((pi < 0) && ((-pi) < GENAUIGKEIT)) ||
	((pi > 0) && (pi < GENAUIGKEIT))) {
	brauchbare_kanten[j].append(e);

	if (schon_eingefuegt[j] == false) {
	  ungescannt.push(j);
	  schon_eingefuegt[j] = true;
	}
      }
    }
  }


  if (!blossomliste.empty()) {
    edge_array <bool >kante_schon_betrachtet(G, false);
    list_item bllit = blossomliste.first();

    while (bllit != nil) {
      if ((blossomliste[bllit].typ == S) && (blossomliste[bllit].d3_kante != nil)) {
	edge e = blossomliste[bllit].d3_kante;

	DUALER_TYP pi = u[G.source(e)] + u[G.target(e)] - gewicht[e];

	if ((pi == 0) ||
	  ((pi < 0) && ((-pi) < GENAUIGKEIT)) ||
	  ((pi > 0) && (pi < GENAUIGKEIT))) {
	  if (schon_eingefuegt[G.source(e)] == false) {
	    ungescannt.push(G.source(e));
	    schon_eingefuegt[G.source(e)] = true;
	  }

	  if (schon_eingefuegt[G.target(e)] == false) {
	    ungescannt.push(G.target(e));
	    schon_eingefuegt[G.target(e)] = true;
	  }

	  if (kante_schon_betrachtet[e] == false) {
	    brauchbare_kanten[G.source(e)].append(e);
	    brauchbare_kanten[G.target(e)].append(e);
	    kante_schon_betrachtet[e] = true;
	  }
	}
      }

      bllit = blossomliste.succ(bllit);
    }
  }

  return erg_aendere;
}

void expandiere_blossoms(
   const ugraph &G,
   const edge_array <GEWICHT_TYP > &gewicht,
   const node_array <DUALER_TYP > &u,
   const edge_array <bool >&gematcht,
   list <blossom > &blossomliste,
   list <blossom > &unterblossomliste,
   list <node >&ungescannt,
   node_array <list_item >&blossom_zu_knoten,
   node_array <KANTENLISTE > &brauchbare_kanten,
   node_array <edge >&d2_kante)
{

  list <list_item >entferne_liste, verschiebe_liste;



  list_item bllit = blossomliste.first();

  while (bllit != nil) {
    if ((!blossomliste[bllit].knotenliste.empty()) &&
      (blossomliste[bllit].typ == T) && (blossomliste[bllit].z == 0)) {



      edge e = blossomliste[bllit].kante;
      node i;

      if (blossom_zu_knoten[G.source(e)] == bllit)
	i = G.source(e);
      else if (blossom_zu_knoten[G.target(e)] == bllit)
	i = G.target(e);



      list_item knotenlit = blossomliste[bllit].knotenliste.first();
      node j = blossomliste[bllit].knotenliste.inf(knotenlit);
      list_item ubzlit = blossomliste[bllit].ub_zeiger_liste.first();
      list_item ubllit = blossomliste[bllit].ub_zeiger_liste.inf(ubzlit);
      list_item kantenlit = blossomliste[bllit].kantenliste.first();


      while (i != j) {
	if (j == unterblossomliste[ubllit].basis) {
	  ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	  ubllit = blossomliste[bllit].ub_zeiger_liste.inf(ubzlit);

	  kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	}

	knotenlit = blossomliste[bllit].knotenliste.succ(knotenlit);
	j = blossomliste[bllit].knotenliste.inf(knotenlit);
      }



      unterblossomliste[ubllit].typ = T;
      unterblossomliste[ubllit].kante = e;


      e = blossomliste[bllit].kantenliste.inf(kantenlit);

      if (gematcht[e] == true) {

	list_item ubzlit_kopie = ubzlit;
	list_item kantenlit_kopie = kantenlit;

	ubzlit = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);

	while (ubzlit != nil) {
	  e = blossomliste[bllit].kantenliste.inf(kantenlit);
	  ubllit = blossomliste[bllit].ub_zeiger_liste.inf(ubzlit);

	  if (gematcht[e] == true) {
	    unterblossomliste[ubllit].typ = S;
	    unterblossomliste[ubllit].kante = e;
	  }

	  else {
	    unterblossomliste[ubllit].typ = T;
	    unterblossomliste[ubllit].kante = e;
	  }

	  kantenlit = blossomliste[bllit].kantenliste.pred(kantenlit);
	  ubzlit = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);
	}


	ubzlit = blossomliste[bllit].ub_zeiger_liste.last();
	ubllit = blossomliste[bllit].ub_zeiger_liste[ubzlit];
	e = blossomliste[bllit].kantenliste.inf(kantenlit);

	unterblossomliste[ubllit].typ = T;
	unterblossomliste[ubllit].kante = e;



	ubzlit = ubzlit_kopie;
	kantenlit = kantenlit_kopie;

	ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);

	while (kantenlit != nil) {

	  e = blossomliste[bllit].kantenliste[kantenlit];

	  list_item ubllit = blossomliste[bllit].ub_zeiger_liste[ubzlit];

	  list_item ubzlit_succ = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	  list_item ubllit_succ = blossomliste[bllit].ub_zeiger_liste[ubzlit_succ];

	  if (!unterblossomliste[ubllit].knotenliste.empty()) {
	    list_item knotenlit = unterblossomliste[ubllit].knotenliste.first();

	    while ((knotenlit != nil) && (unterblossomliste[ubllit].typ == UNMARKIERT)) {
	      node i = unterblossomliste[ubllit].knotenliste[knotenlit];

	      if (!brauchbare_kanten[i].empty()) {
		edge e_brauchbar = brauchbare_kanten[i].pop();

		unterblossomliste[ubllit].typ = T;
		unterblossomliste[ubllit].kante = e_brauchbar;
		unterblossomliste[ubllit_succ].typ = S;
		unterblossomliste[ubllit_succ].kante = e;
	      }

	      knotenlit = unterblossomliste[ubllit].knotenliste.succ(knotenlit);
	    }
	  }

	  else {
	    node i = unterblossomliste[ubllit].basis;

	    if (!brauchbare_kanten[i].empty()) {
	      edge e_brauchbar = brauchbare_kanten[i].pop();

	      unterblossomliste[ubllit].typ = T;
	      unterblossomliste[ubllit].kante = e_brauchbar;
	      unterblossomliste[ubllit_succ].typ = S;
	      unterblossomliste[ubllit_succ].kante = e;
	    }
	  }


	  if (!unterblossomliste[ubllit_succ].knotenliste.empty()) {
	    list_item knotenlit = unterblossomliste[ubllit_succ].knotenliste.first();

	    while ((knotenlit != nil) &&
	      (unterblossomliste[ubllit_succ].typ == UNMARKIERT)) {
	      node i = unterblossomliste[ubllit_succ].knotenliste[knotenlit];

	      if (!brauchbare_kanten[i].empty()) {
		edge e_brauchbar = brauchbare_kanten[i].pop();

		unterblossomliste[ubllit_succ].typ = T;
		unterblossomliste[ubllit_succ].kante = e_brauchbar;
		unterblossomliste[ubllit].typ = S;
		unterblossomliste[ubllit].kante = e;
	      }

	      knotenlit = unterblossomliste[ubllit_succ].knotenliste.succ(knotenlit);
	    }
	  }

	  else {
	    node i = unterblossomliste[ubllit_succ].basis;

	    if (!brauchbare_kanten[i].empty()) {
	      edge e_brauchbar = brauchbare_kanten[i].pop();

	      unterblossomliste[ubllit_succ].typ = T;
	      unterblossomliste[ubllit_succ].kante = e_brauchbar;
	      unterblossomliste[ubllit].typ = S;
	      unterblossomliste[ubllit].kante = e;
	    }
	  }



	  ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	  ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	  kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	  kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	}


      }


      else if (gematcht[e] != true) {

	list_item ubzlit_kopie = ubzlit;
	list_item kantenlit_kopie = kantenlit;

	kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);

	while (kantenlit != nil) {
	  e = blossomliste[bllit].kantenliste.inf(kantenlit);
	  ubllit = blossomliste[bllit].ub_zeiger_liste.inf(ubzlit);

	  if (gematcht[e] == true) {
	    unterblossomliste[ubllit].typ = S;
	    unterblossomliste[ubllit].kante = e;
	  }

	  else {
	    unterblossomliste[ubllit].typ = T;
	    unterblossomliste[ubllit].kante = e;
	  }

	  kantenlit = blossomliste[bllit].kantenliste.succ(kantenlit);
	  ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
	}



	ubzlit = ubzlit_kopie;
	kantenlit = kantenlit_kopie;

	ubzlit = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);
	kantenlit = blossomliste[bllit].kantenliste.pred(kantenlit);

	while (kantenlit != nil) {

	  e = blossomliste[bllit].kantenliste[kantenlit];

	  list_item ubllit = blossomliste[bllit].ub_zeiger_liste[ubzlit];

	  list_item ubzlit_pred = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);
	  list_item ubllit_pred = blossomliste[bllit].ub_zeiger_liste[ubzlit_pred];

	  if (!unterblossomliste[ubllit].knotenliste.empty()) {
	    list_item knotenlit = unterblossomliste[ubllit].knotenliste.first();

	    while ((knotenlit != nil) && (unterblossomliste[ubllit].typ == UNMARKIERT)) {
	      node i = unterblossomliste[ubllit].knotenliste[knotenlit];

	      if (!brauchbare_kanten[i].empty()) {
		edge e_brauchbar = brauchbare_kanten[i].pop();

		unterblossomliste[ubllit].typ = T;
		unterblossomliste[ubllit].kante = e_brauchbar;
		unterblossomliste[ubllit_pred].typ = S;
		unterblossomliste[ubllit_pred].kante = e;
	      }

	      knotenlit = unterblossomliste[ubllit].knotenliste.succ(knotenlit);
	    }
	  }

	  else {
	    node i = unterblossomliste[ubllit].basis;

	    if (!brauchbare_kanten[i].empty()) {
	      edge e_brauchbar = brauchbare_kanten[i].pop();

	      unterblossomliste[ubllit].typ = T;
	      unterblossomliste[ubllit].kante = e_brauchbar;
	      unterblossomliste[ubllit_pred].typ = S;
	      unterblossomliste[ubllit_pred].kante = e;
	    }
	  }


	  if (!unterblossomliste[ubllit_pred].knotenliste.empty()) {
	    list_item knotenlit = unterblossomliste[ubllit_pred].knotenliste.first();

	    while ((knotenlit != nil) &&
	      (unterblossomliste[ubllit_pred].typ == UNMARKIERT)) {
	      node i = unterblossomliste[ubllit_pred].knotenliste[knotenlit];

	      if (!brauchbare_kanten[i].empty()) {
		edge e_brauchbar = brauchbare_kanten[i].pop();

		unterblossomliste[ubllit_pred].typ = T;
		unterblossomliste[ubllit_pred].kante = e_brauchbar;
		unterblossomliste[ubllit].typ = S;
		unterblossomliste[ubllit].kante = e;
	      }

	      knotenlit = unterblossomliste[ubllit_pred].knotenliste.succ(knotenlit);
	    }
	  }

	  else {
	    node i = unterblossomliste[ubllit_pred].basis;

	    if (!brauchbare_kanten[i].empty()) {
	      edge e_brauchbar = brauchbare_kanten[i].pop();

	      unterblossomliste[ubllit_pred].typ = T;
	      unterblossomliste[ubllit_pred].kante = e_brauchbar;
	      unterblossomliste[ubllit].typ = S;
	      unterblossomliste[ubllit].kante = e;
	    }
	  }



	  ubzlit = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);
	  ubzlit = blossomliste[bllit].ub_zeiger_liste.pred(ubzlit);
	  kantenlit = blossomliste[bllit].kantenliste.pred(kantenlit);
	  kantenlit = blossomliste[bllit].kantenliste.pred(kantenlit);
	}


      }





      ubzlit = blossomliste[bllit].ub_zeiger_liste.first();

      while (ubzlit != nil) {
	ubllit = blossomliste[bllit].ub_zeiger_liste.inf(ubzlit);
	verschiebe_liste.append(ubllit);

	ubzlit = blossomliste[bllit].ub_zeiger_liste.succ(ubzlit);
      }



      entferne_liste.append(bllit);




    }

    bllit = blossomliste.succ(bllit);
  }



  while (!entferne_liste.empty()) {
    blossomliste.del_item(entferne_liste.pop());
  }



  if (!verschiebe_liste.empty()) {
    list_item vlit = verschiebe_liste.first();
    list_item ubllit;

    while (vlit != nil) {
      ubllit = verschiebe_liste.inf(vlit);
      list_item hilfslit = blossomliste.append(unterblossomliste[ubllit]);
      unterblossomliste.del_item(verschiebe_liste.inf(vlit));


      if (!blossomliste[hilfslit].knotenliste.empty()) {
	list_item knotenlit = blossomliste[hilfslit].knotenliste.first();

	while (knotenlit != nil) {
	  node i = blossomliste[hilfslit].knotenliste[knotenlit];
	  blossom_zu_knoten[i] = hilfslit;

	  knotenlit = blossomliste[hilfslit].knotenliste.succ(knotenlit);
	}
      }

      else {
	node i = blossomliste[hilfslit].basis;
	blossom_zu_knoten[i] = hilfslit;
      }

      vlit = verschiebe_liste.succ(vlit);
    }
  }


  if (!verschiebe_liste.empty()) {
    list_item vlit = verschiebe_liste.first();
    list_item hilfslit = blossomliste.last();

    while (vlit != nil) {
      if (blossomliste[hilfslit].typ == S) {
	neues_s_blossom
	  (G, gewicht, u, blossomliste, blossom_zu_knoten, gematcht,
	  blossomliste[hilfslit], ungescannt, brauchbare_kanten, d2_kante);
      }

      hilfslit = blossomliste.pred(hilfslit);
      vlit = verschiebe_liste.succ(vlit);
    }
  }


}




static void matchingtest(const ugraph &G, const edge_array <bool >&gematcht)
{
  node_array <bool >knoten_betrachtet(G, false);
  edge e;

  forall_edges (e, G) {
    node i = G.source(e);
    node j = G.target(e);

    if (gematcht[e] == true) {
      if (knoten_betrachtet[i] == false)
	knoten_betrachtet[i] = true;

      else {
	cout << "Knoten ";
	G.print_node(i);
	cout << " ist Endknoten mehrerer gematchter Kanten!!!\n";
      }

      if (knoten_betrachtet[j] == false)
	knoten_betrachtet[j] = true;

      else {
	cout << "Knoten ";
	G.print_node(j);
	cout << " ist Endknoten mehrerer gematchter Kanten!!!\n";
      }
    }
  }

  cout << "Test auf Matching abgeschlossen!\n";
  return;
}



static void bed1test(const ugraph &G, const edge_array <GEWICHT_TYP > &gewicht, const
   node_array <DUALER_TYP > &u, const list <blossom > &blossomliste, const
   list <blossom > &unterblossomliste, const node_array <list_item >&blossom_zu_knoten)
{

  node i;

  forall_nodes (i, G) {
    if (u[i] < 0) {
      cout << "Die duale Variable von Knoten ";
      G.print_node(i);
      cout << " ist negativ\n";
    }
  }



  edge e;

  forall_edges (e, G) {
    node i = G.source(e);
    node j = G.target(e);

    if (i==j) continue;

    list_item litsource = blossom_zu_knoten[i];
    list_item littarget = blossom_zu_knoten[j];
    DUALER_TYP z;

    if (litsource != littarget)
      z = 0;

    else
      z = bestimme_z(G, blossomliste, unterblossomliste, blossomliste[litsource], e);

    DUALER_TYP pi = u[i] + u[j] - gewicht[e] + z;

    if (pi < 0) {
      cout << "pi<0 fuer Kante ";
      G.print_edge(e);
      cout << endl;
    }
  }




  if (!blossomliste.empty()) {
    list_item lit = blossomliste.first();

    while (lit != nil) {
      if (blossomliste[lit].z < 0) {
	cout << "Es gibt ein z<0\n";
      }

      lit = blossomliste.succ(lit);
    }
  }



  if (!unterblossomliste.empty()) {
    list_item lit = unterblossomliste.first();

    while (lit != nil) {
      if (unterblossomliste[lit].z < 0) {
	cout << "Es gibt ein z<0\n";
      }

      lit = unterblossomliste.succ(lit);
    }
  }





  cout << "Test von Bedingung (1) abgeschlossen!\n";
  return;
}



static void bed2test(const ugraph &G, const edge_array <GEWICHT_TYP > &gewicht, const
   list <blossom > &blossomliste, const list <blossom > &unterblossomliste, const
   node_array <list_item >&blossom_zu_knoten, const edge_array <bool >&gematcht,
   const node_array <DUALER_TYP > &u)
{
  edge e;

  forall_edges (e, G) {
    if (gematcht[e] == true) {
      node i = G.source(e);
      node j = G.target(e);
      list_item litsource = blossom_zu_knoten[i];
      list_item littarget = blossom_zu_knoten[j];
      DUALER_TYP z;

      if (litsource != littarget)
	z = 0;

      else
	z = bestimme_z(G, blossomliste, unterblossomliste, blossomliste[litsource], e);

      DUALER_TYP pi = u[i] + u[j] - gewicht[e] + z;

      if (pi > 0) {
	cout << "Kante ";
	G.print_edge(e);
	cout << " ist gematcht, aber pi ist positiv\n";
      }
    }
  }

  cout << "Test von Bedingung (2) abgeschlossen!\n";
  return;
}



static void bed3test(const ugraph &G, const node_array <DUALER_TYP > &u, const
   node_array <edge >&gematchte_kante)
{
  node i;

  forall_nodes (i, G) {
    if (u[i] > 0) {
      if (gematchte_kante[i] == nil) {
	cout << "Die duale Variable des freien Knotens ";
	G.print_node(i);
	cout << " ist positiv\n";
      }
    }
  }

  cout << "Test von Bedingung (3) abgeschlossen!\n";
  return;
}



static void bed4test(const ugraph &G, const list <blossom > &blossomliste, const
   list <blossom > &unterblossomliste, const node_array <edge >&gematchte_kante,
   const node_array <list_item >&blossom_zu_knoten)
{

  if (!blossomliste.empty()) {
    list_item lit = blossomliste.first();

    while (lit != nil) {
      node_array <bool >schon_besucht(G, false);
      int anzknoten = 0;
      int anzgematchtekanten = 0;

      if ((blossomliste[lit].z > 0) &&
	(!blossomliste[lit].knotenliste.empty())) {

	list_item knotenlit = blossomliste[lit].knotenliste.first();

	while (knotenlit != nil) {
	  anzknoten++;
	  node v = blossomliste[lit].knotenliste[knotenlit];

	  if ((!schon_besucht[v]) && (gematchte_kante[v] != nil)) {
	    schon_besucht[v] = true;

	    edge e = gematchte_kante[v];
	    node w = G.opposite(v, e);

	    if (blossom_zu_knoten[w] == lit) {
	      anzgematchtekanten++;
	      schon_besucht[w] = true;
	    }
	  }

	  knotenlit = blossomliste[lit].knotenliste.succ(knotenlit);
	}

	if (anzknoten != (2 * anzgematchtekanten + 1)) {
	  cout << "Bedingung (4) ist verletzt!!!\n";
	}


      }

      lit = blossomliste.succ(lit);
    }
  }



  if (!unterblossomliste.empty()) {
    list_item lit = unterblossomliste.first();

    while (lit != nil) {
      node_array <bool >schon_besucht(G, false);
      int anzknoten = 0;
      int anzgematchtekanten = 0;

      if ((unterblossomliste[lit].z > 0) &&
	(!unterblossomliste[lit].knotenliste.empty())) {

	list_item knotenlit = unterblossomliste[lit].knotenliste.first();

	while (knotenlit != nil) {
	  anzknoten++;
	  node v = unterblossomliste[lit].knotenliste[knotenlit];

	  if ((!schon_besucht[v]) && (gematchte_kante[v] != nil)) {
	    schon_besucht[v] = true;

	    edge e = gematchte_kante[v];
	    node w = G.opposite(v, e);

	    list_item knotenlit2 = unterblossomliste[lit].knotenliste.first();

	    while (knotenlit2 != nil) {
	      if (unterblossomliste[lit].knotenliste[knotenlit2] == w) {
		break;
	      }

	      knotenlit2 = unterblossomliste[lit].knotenliste.succ(knotenlit2);
	    }

	    if (knotenlit2 != nil) {
	      anzgematchtekanten++;
	      schon_besucht[w] = true;
	    }
	  }

	  knotenlit = unterblossomliste[lit].knotenliste.succ(knotenlit);
	}

	if (anzknoten != (2 * anzgematchtekanten + 1)) {
	  cout << "Bedingung (4) ist verletzt!!!\n";
	}


      }

      lit = unterblossomliste.succ(lit);
    }
  }



  cout << "Test von Bedingung (4) abgeschlossen!\n";
  return;
}



static DUALER_TYP bestimme_z(const ugraph &G, const list <blossom > &blossomliste,
   const list <blossom > &unterblossomliste, const blossom & bl, const edge &e)
{

  DUALER_TYP z = bl.z;

  if (!bl.ub_zeiger_liste.empty()) {
    list_item ubzlit = bl.ub_zeiger_liste.first();

    while (ubzlit != nil) {
      int anzahl = 0;
      list_item ubllit = bl.ub_zeiger_liste[ubzlit];

      if (!unterblossomliste[ubllit].knotenliste.empty()) {
	list_item knotenlit = unterblossomliste[ubllit].knotenliste.first();

	while (knotenlit != nil) {
	  node k = unterblossomliste[ubllit].knotenliste[knotenlit];

	  if ((k == G.source(e)) || (k == G.target(e)))
	    anzahl++;
	  if (anzahl == 2)
	    break;

	  knotenlit = unterblossomliste[ubllit].knotenliste.succ(knotenlit);
	}
      }

      if (anzahl == 2) {
	z += bestimme_z(G, blossomliste, unterblossomliste,
	  unterblossomliste[ubllit], e);
	break;
      }

      else if (anzahl == 1)
	break;

      ubzlit = bl.ub_zeiger_liste.succ(ubzlit);
    }
  }

  return z;
}
