/* Fichier banque.c */
/* s'occupe de tout ce qui concerne les banques */


/*     Copyright (C) 2000-2003  C�dric Auger */
/* 			cedric@grisbi.org */
/* 			http://www.grisbi.org */

/*     This program is free software; you can redistribute it and/or modify */
/*     it under the terms of the GNU General Public License as published by */
/*     the Free Software Foundation; either version 2 of the License, or */
/*     (at your option) any later version. */

/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "en_tete.h"



/* ***************************************************************************************************** */
/* Fonction ajout_banque */
/* appel�e par le bouton ajouter de l'onglet banques des param�tres */
/* ***************************************************************************************************** */

void ajout_banque ( GtkWidget *bouton,
		    GtkWidget *clist )
{
  struct struct_banque *banque;
  gchar *ligne[1];
  gint ligne_insert;

/* enl�ve la s�lection de la liste ( ce qui nettoie les entr�es ) */

  gtk_clist_unselect_all ( GTK_CLIST ( clist ));


/* cr�e une nouvelle banque au nom de "nouvelle banque" en mettant tous les param�tres � 0 et le no � -1 */

  banque = calloc ( 1,
		    sizeof ( struct struct_banque ));

  banque -> no_banque = -1;
  banque -> nom_banque = g_strdup ( _("New bank") );

  liste_struct_banques_tmp = g_slist_append ( liste_struct_banques_tmp,
					      banque );

  ligne[0] = banque -> nom_banque;

  ligne_insert = gtk_clist_append ( GTK_CLIST ( clist ),
				    ligne );

  /* on associe � la ligne la struct de la banque */

  gtk_clist_set_row_data ( GTK_CLIST ( clist ),
			   ligne_insert,
			   banque );

/* on s�lectionne le nouveau venu */

  gtk_clist_select_row ( GTK_CLIST ( clist ),
			 ligne_insert,
			 0 );

/* on s�lectionne le "nouvelle banque" et lui donne le focus */

  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     TRUE );
  gtk_entry_select_region ( GTK_ENTRY ( nom_banque ),
			    0,
			    -1 );
  gtk_widget_grab_focus ( nom_banque );


}
/* ***************************************************************************************************** */



/* ***************************************************************************************************** */
/* Fonction applique_modif_banque */
/* appel�e par le bouton appliquer qui devient sensitif lorsqu'on modifie la banque des param�tres */
/* ***************************************************************************************************** */

void applique_modif_banque ( GtkWidget *liste )
{
  struct struct_banque *banque;

  banque = gtk_clist_get_row_data ( GTK_CLIST ( liste ),
				    ligne_selection_banque );

  if ( !strlen ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( nom_banque )))))
    {
      dialogue ( _("You must name this bank.") );
      return;
    }

  if ( banque -> no_banque == -1 )
    {
      banque -> no_banque = ++no_derniere_banque_tmp;
      nb_banques_tmp++;
    }

  banque -> nom_banque = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( nom_banque ))));

  banque -> code_banque = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( code_banque ))));
  if ( !strlen ( banque -> code_banque ))
    banque -> code_banque = NULL;

  banque -> tel_banque = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( tel_banque ))));
  if ( !strlen ( banque -> tel_banque ))
    banque -> tel_banque = NULL;

  banque -> email_banque = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( email_banque ))));
  if ( !strlen ( banque -> email_banque ))
    banque -> email_banque = NULL;

  banque -> web_banque = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( web_banque ))));
  if ( !strlen ( banque -> web_banque ))
    banque -> web_banque = NULL;


  banque -> nom_correspondant = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( nom_correspondant ))));
  if ( !strlen ( banque -> nom_correspondant ))
    banque -> nom_correspondant = NULL;

  banque -> tel_correspondant = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( tel_correspondant ))));
  if ( !strlen ( banque -> tel_correspondant ))
    banque -> tel_correspondant = NULL;

  banque -> email_correspondant = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( email_correspondant ))));
  if ( !strlen ( banque -> email_correspondant ))
    banque -> email_correspondant = NULL;

  banque -> fax_correspondant = g_strdup ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( fax_correspondant ))));
  if ( !strlen ( banque -> fax_correspondant ))
    banque -> fax_correspondant = NULL;

  banque -> adr_banque = g_strdup ( g_strstrip ( gtk_editable_get_chars (GTK_EDITABLE ( adr_banque ),
									 0,
									 -1 )) );
    if ( strlen ( banque -> adr_banque ))
      {
	banque -> adr_banque = g_strdelimit ( banque -> adr_banque,
					      "{",
					      '(' );
	banque -> adr_banque = g_strdelimit ( banque -> adr_banque,
					      "}",
					      ')' );
      }
    else
      banque -> adr_banque = NULL;


  banque -> remarque_banque = g_strdup ( g_strstrip ( gtk_editable_get_chars (GTK_EDITABLE ( remarque_banque ),
									 0,
									      -1 )) );
    if ( strlen ( banque -> remarque_banque ))
      {
	banque -> remarque_banque = g_strdelimit ( banque -> remarque_banque,
					      "{",
					      '(' );
	banque -> remarque_banque = g_strdelimit ( banque -> remarque_banque,
					      "}",
					      ')' );
      }
    else
      banque -> remarque_banque = NULL;

  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     FALSE );

/* met le nom dans la liste */

  gtk_clist_set_text ( GTK_CLIST ( liste ),
		       ligne_selection_banque,
		       0,
		       banque -> nom_banque );
  gtk_clist_set_text ( GTK_CLIST ( liste ),
		       ligne_selection_banque,
		       1,
		       banque -> nom_correspondant );

  gnome_property_box_changed ( GNOME_PROPERTY_BOX ( fenetre_preferences));
}
/* ***************************************************************************************************** */


/* **************************************************************************************************************************** */
/* Fonction annuler_modif_banque */
/* appel�e lorsqu'on clicke sur le bouton annuler dans les param�tres */
/* **************************************************************************************************************************** */

void annuler_modif_banque ( GtkWidget *bouton,
			      GtkWidget *liste )
{
  struct struct_banque *banque;

  banque = gtk_clist_get_row_data ( GTK_CLIST ( clist_banques_parametres ),
				    ligne_selection_banque );

/* si c'�tait une nouvelle banque, on la supprime */

  if ( banque -> no_banque == -1 )
    {
      gtk_clist_unselect_all ( GTK_CLIST ( clist_banques_parametres ));
      return;
      gtk_clist_remove ( GTK_CLIST ( clist_banques_parametres ),
			 ligne_selection_banque );
      liste_struct_banques_tmp = g_slist_remove ( liste_struct_banques_tmp,
						  banque );
      free ( banque );
      return;
    }

/* sinon, on remplit tous les champs avec l'ancienne banque */

  gtk_entry_set_text ( GTK_ENTRY ( nom_banque ),
		       banque -> nom_banque );

  if ( banque -> code_banque )
    gtk_entry_set_text ( GTK_ENTRY ( code_banque ),
			 banque -> code_banque );
  else
    gtk_entry_set_text ( GTK_ENTRY ( code_banque ),
			 "" );
  
  if ( banque -> tel_banque )
    gtk_entry_set_text ( GTK_ENTRY ( tel_banque ),
			 banque -> tel_banque );
  else
    gtk_entry_set_text ( GTK_ENTRY ( tel_banque ),
			 "" );

  if ( banque -> email_banque )
    gtk_entry_set_text ( GTK_ENTRY ( email_banque ),
			 banque -> email_banque  );
  else
    gtk_entry_set_text ( GTK_ENTRY ( email_banque ),
			 "" );

  if ( banque -> web_banque )
    gtk_entry_set_text ( GTK_ENTRY ( web_banque ),
			 banque -> web_banque );
  else
    gtk_entry_set_text ( GTK_ENTRY ( web_banque ),
			 "" );

  if ( banque -> nom_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( nom_correspondant ),
			 banque -> nom_correspondant );
  else
    gtk_entry_set_text ( GTK_ENTRY ( nom_correspondant ),
			 "" );

  if ( banque -> tel_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( tel_correspondant ),
			 banque -> tel_correspondant );
  else
    gtk_entry_set_text ( GTK_ENTRY ( tel_correspondant ),
			 "" );

  if ( banque -> email_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( email_correspondant ),
			 banque -> email_correspondant );
  else
    gtk_entry_set_text ( GTK_ENTRY ( email_correspondant ),
			 "" );

  if ( banque -> fax_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( fax_correspondant ),
			 banque -> fax_correspondant );
  else
    gtk_entry_set_text ( GTK_ENTRY ( fax_correspondant ),
			 "" );

  gtk_text_set_point ( GTK_TEXT ( adr_banque ),
		       0 );
  gtk_text_forward_delete ( GTK_TEXT ( adr_banque ),
			    gtk_text_get_length ( GTK_TEXT ( adr_banque ) ) );
  if ( banque -> adr_banque )
    {
      gtk_text_insert ( GTK_TEXT ( adr_banque ),
			NULL,
			NULL,
			NULL,
			banque -> adr_banque,
			-1 );
    }

  gtk_text_set_point ( GTK_TEXT ( remarque_banque ),
		       0 );
  gtk_text_forward_delete ( GTK_TEXT ( remarque_banque ),
			    gtk_text_get_length ( GTK_TEXT ( remarque_banque ) ) );
  if ( banque -> remarque_banque )
    gtk_text_insert ( GTK_TEXT ( remarque_banque ),
		      NULL,
		      NULL,
		      NULL,
		      banque -> remarque_banque,
		      -1 );
  else
    gtk_entry_set_text ( GTK_ENTRY ( remarque_banque ),
			 "" );

  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     FALSE );

}
/* **************************************************************************************************************************** */



/* **************************************************************************************************************************** */
/* Fonction supprime_banque */
/* appel�e lorsqu'on clicke sur le bouton annuler dans les param�tres */
/* **************************************************************************************************************************** */

void supprime_banque ( GtkWidget *bouton,
		       GtkWidget *liste )
{
  struct struct_banque *banque;
  GtkWidget *dialogue;
  GtkWidget *label;
  gint resultat;

  banque = gtk_clist_get_row_data ( GTK_CLIST ( liste ),
				    ligne_selection_banque );

  dialogue = gnome_dialog_new ( _("Confirmation of bank removal"),
				GNOME_STOCK_BUTTON_YES,
				GNOME_STOCK_BUTTON_NO,
				NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialogue ),
				 GTK_WINDOW ( fenetre_preferences ));

  label = gtk_label_new ( g_strdup_printf ( _("Are you sure you want to remove bank '%s'?\n"),
					    banque -> nom_banque));
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialogue ) -> vbox ),
		       label,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( label );

  resultat = gnome_dialog_run_and_close ( GNOME_DIALOG ( dialogue ));

  if ( !resultat )
    {
      /* on d�sensitive la hbox_boutons_modif_banque au cas o� on �tait en train de modifier */
      /* la banque */

      gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
				 FALSE );

      gtk_clist_remove ( GTK_CLIST ( liste ),
			 ligne_selection_banque );
      liste_struct_banques_tmp = g_slist_remove ( liste_struct_banques_tmp,
						  banque );
      free ( banque );
      nb_banques_tmp--;
      gnome_property_box_changed ( GNOME_PROPERTY_BOX ( fenetre_preferences));
    }

}
/* **************************************************************************************************************************** */





/* ************************************************************************************************************ */
/* Fonction creation_menu_banques */
/* cr�e un menu qui contient les noms des banques associ�s � leur no et adr */
/* et le renvoie */
/* ************************************************************************************************************ */

GtkWidget *creation_menu_banques ( void )
{
  GtkWidget *menu;
  GtkWidget *menu_item;
  GSList *pointeur;

  menu = gtk_menu_new ();
  gtk_signal_connect ( GTK_OBJECT ( menu ),
		       "selection-done",
		       GTK_SIGNAL_FUNC ( changement_de_banque ),
		       NULL );
  gtk_widget_show ( menu );


  /* le premier nom est Aucune */

  menu_item = gtk_menu_item_new_with_label ( _("None") );
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"adr_banque",
			NULL );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_banque",
			NULL );
  gtk_widget_show ( menu_item );


  pointeur = liste_struct_banques;

  while ( pointeur )
    {
      struct struct_banque *banque;

      banque = pointeur -> data;

      menu_item = gtk_menu_item_new_with_label ( banque -> nom_banque );
      gtk_menu_append ( GTK_MENU ( menu ),
			menu_item );
      gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			    "adr_banque",
			    banque );
      gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			    "no_banque",
			    GINT_TO_POINTER ( banque -> no_banque ));
      gtk_widget_show ( menu_item );

      pointeur = pointeur -> next;
    }

  return ( menu );
}
/* ************************************************************************************************************ */


/* **************************************************************************************************************************** */
/* Fonction affiche_detail_banque */
/* appel�e par le bouton "d�tails" des d�tails de comptes */
/* cr�e une fen�tre avec les coordonn�es de la banque */
/* **************************************************************************************************************************** */

void affiche_detail_banque ( GtkWidget *bouton,
			     gpointer null )
{
  GtkWidget *dialogue;
  GtkWidget *table;
  GtkWidget *label;
  struct struct_banque *banque;
  GtkWidget *separateur;

  banque = gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( detail_option_menu_banque ) -> menu_item ),
				 "adr_banque" );

  if ( !banque )
    return;

  dialogue = gnome_dialog_new ( _("Information about the bank"),
				GNOME_STOCK_BUTTON_OK,
				GNOME_STOCK_PIXMAP_PROPERTIES,
				NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialogue ),
				 GTK_WINDOW ( window ));



  table = gtk_table_new ( 2,
			  11,
			  FALSE );
  gtk_table_set_row_spacings ( GTK_TABLE ( table ),
			       10 );
  gtk_table_set_col_spacings ( GTK_TABLE ( table ),
			       10 );
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialogue ) -> vbox ),
		       table,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( table );


  /* mise en place du nom */

  label = gtk_label_new ( COLON(_("Bank")) );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1, 0, 1 );			     
  gtk_widget_show ( label );

  label = gtk_label_new ( banque -> nom_banque );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      1, 2, 0, 1 );			     
  gtk_widget_show ( label );


  /* mise en place de l'adresse */

  if ( banque -> adr_banque )
    {
      label = gtk_label_new ( banque -> adr_banque );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 1, 2 );			     
      gtk_widget_show ( label );
    }



  /* mise en place du t�l�phone */

  if ( banque -> tel_banque )
    {
      label = gtk_label_new ( COLON(_("Phone number")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 2, 3 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> tel_banque );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 2, 3 );			     
      gtk_widget_show ( label );
    }


  if ( banque -> email_banque )
    {
      label = gtk_label_new ( COLON(_("E-Mail")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 3, 4 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> email_banque );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 3, 4 );			     
      gtk_widget_show ( label );
    }
      
  if ( banque -> web_banque )
    {
      label = gtk_label_new ( COLON(_("Website")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 4, 5 );			     
      gtk_widget_show ( label );

      label = gnome_href_new ( banque -> web_banque,
			       banque -> web_banque );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 4, 5 );			     
      gtk_widget_show ( label );
    }


  /* mise en place du correspondant */

  separateur = gtk_hseparator_new ();
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      separateur,
			      0, 2, 5, 6 );			     
  gtk_widget_show ( separateur );


  if ( banque -> nom_correspondant )
    {
      /* mise en place du nom */

      label = gtk_label_new ( COLON(_("Contact name")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 6, 7 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> nom_correspondant );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 6, 7 );			     
      gtk_widget_show ( label );
    }

  /* mise en place du t�l�phone */

  if ( banque -> tel_correspondant )
    {
      label = gtk_label_new ( COLON(_("Phone number")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 7, 8 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> tel_correspondant );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 7, 8 );			     
      gtk_widget_show ( label );
    }

  /* mise en place du fax */

  if ( banque -> fax_correspondant )
    {
      label = gtk_label_new ( COLON(_("Fax")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 8, 9 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> fax_correspondant );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 8, 9 );			     
      gtk_widget_show ( label );
    }


  if ( banque -> email_correspondant )
    {
      label = gtk_label_new ( COLON(_("E-Mail")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 9, 10 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> email_correspondant );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 9, 10 );			     
      gtk_widget_show ( label );
    }
      


  if ( banque -> remarque_banque )
    {
      label = gtk_label_new ( COLON(_("Notes")) );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  0, 1, 10, 11 );			     
      gtk_widget_show ( label );

      label = gtk_label_new ( banque -> remarque_banque );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0 );
      gtk_table_attach_defaults ( GTK_TABLE ( table ),
				  label,
				  1, 2, 10, 11 );			     
      gtk_widget_show ( label );
    }

  switch (gnome_dialog_run_and_close ( GNOME_DIALOG ( dialogue )))
    {
    case 0:			/* OK */
      return;
    case 1:			/* Properties */
      preferences ( (GtkWidget *) NULL, 6 );
      gtk_clist_select_row ( (GtkCList *) clist_banques_parametres, banque -> no_banque-1, 0 );
      return;
    case -1:			/* Something went wrong or user closed
				   dialog with window manager */
      return;
    }

}
/* **************************************************************************************************************************** */





/**
 * Creates the "Banks" tab.  It creates a bank list and then a form
 * that allows to edit selected bank.
 *
 * \returns A newly allocated vbox
 */
GtkWidget *onglet_banques ( void )
{
  GtkWidget *vbox_pref, *separateur, *label;
  GtkWidget *scrolled_window, *vbox, *vbox2, *hvbox;
  GtkWidget *bouton, *hbox, *paddingbox, *table;
  GSList *liste_tmp;
  gchar *bank_cols_titles [2] = {_("Bank"),
				 _("Contact name") };

  vbox_pref = new_vbox_with_title_and_icon ( _("Banks"),
					     "banks.png" );


  /* Create bank list */
  paddingbox = paddingbox_new_with_title ( vbox_pref, FALSE,
					   _("Known banks") );
  scrolled_window = gtk_scrolled_window_new ( NULL, NULL );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_NEVER,
				   GTK_POLICY_AUTOMATIC);
  clist_banques_parametres = gtk_clist_new_with_titles ( 2,
							 bank_cols_titles);
  gtk_widget_set_usize ( clist_banques_parametres,
			 FALSE, 100 );
  gtk_clist_set_column_auto_resize ( GTK_CLIST ( clist_banques_parametres ) ,
				      0, TRUE );
  gtk_clist_set_column_auto_resize ( GTK_CLIST ( clist_banques_parametres ) ,
				      1, TRUE );
  gtk_signal_connect_object  ( GTK_OBJECT ( fenetre_preferences ),
			       "apply",
			       GTK_SIGNAL_FUNC ( gtk_clist_unselect_all ),
			       GTK_OBJECT ( clist_banques_parametres ));
  gtk_container_add ( GTK_CONTAINER ( scrolled_window ),
		      clist_banques_parametres );

  hbox = gtk_hbox_new ( FALSE, 10 );
  gtk_box_pack_start ( GTK_BOX ( paddingbox ), hbox,
		       FALSE, FALSE, 0);
  gtk_box_pack_start ( GTK_BOX ( hbox ), scrolled_window,
		       TRUE, TRUE, 0);

  /* Do not activate unless an account is opened */
  if ( !nb_comptes )
    gtk_widget_set_sensitive ( vbox_pref, FALSE );
  else
    {
      /* Work an a copy of original liste_struct_devises */
      liste_struct_banques_tmp = NULL;
      liste_tmp = liste_struct_banques;

      while ( liste_tmp )
	{
	  struct struct_banque *banque;
	  struct struct_banque *copie_banque;

	  banque = liste_tmp -> data;
	  copie_banque = malloc ( sizeof ( struct struct_banque ));

	  copie_banque -> no_banque = banque -> no_banque;
	  copie_banque -> nom_banque = g_strdup ( banque -> nom_banque );
	  copie_banque -> code_banque = g_strdup ( banque -> code_banque );
	  copie_banque -> adr_banque = g_strdup ( banque -> adr_banque );
	  copie_banque -> tel_banque = g_strdup ( banque -> tel_banque );
	  copie_banque -> email_banque = g_strdup ( banque -> email_banque );
	  copie_banque -> web_banque = g_strdup ( banque -> web_banque );
	  copie_banque -> nom_correspondant = g_strdup ( banque -> nom_correspondant );
	  copie_banque -> fax_correspondant = g_strdup ( banque -> fax_correspondant );
	  copie_banque -> tel_correspondant = g_strdup ( banque -> tel_correspondant );
	  copie_banque -> email_correspondant = g_strdup ( banque -> email_correspondant );
	  copie_banque -> remarque_banque = g_strdup ( banque -> remarque_banque );

	  liste_struct_banques_tmp = g_slist_append ( liste_struct_banques_tmp,
						      copie_banque );
	  liste_tmp = liste_tmp -> next;
	}

      no_derniere_banque_tmp = no_derniere_banque;
      nb_banques_tmp = nb_banques;


      /* remplissage de la liste avec les banques temporaires */

      liste_tmp = liste_struct_banques_tmp;

      while ( liste_tmp )
	{
	  struct struct_banque *banque;
	  gchar *ligne[2];
	  gint ligne_insert;

	  banque = liste_tmp -> data;

	  ligne[0] = banque -> nom_banque;
	  ligne[1] = banque -> nom_correspondant;

	  ligne_insert = gtk_clist_append ( GTK_CLIST ( clist_banques_parametres ),
					    ligne );

	  /* on associe � la ligne la struct de la banque */

	  gtk_clist_set_row_data ( GTK_CLIST ( clist_banques_parametres ),
				   ligne_insert,
				   banque );

	  liste_tmp = liste_tmp -> next;
	}
    }

  /* Handle Add & Remove buttons */
  vbox = gtk_vbox_new ( FALSE, 5 );
  gtk_box_pack_start ( GTK_BOX ( hbox ), vbox,
		       FALSE, FALSE, 0 );
  /* Add button */
  bouton = gtk_button_new_from_stock (GTK_STOCK_ADD);
  gtk_signal_connect ( GTK_OBJECT ( bouton ), "clicked",
		       GTK_SIGNAL_FUNC  ( ajout_banque ),
		       clist_banques_parametres );
  gtk_box_pack_start ( GTK_BOX ( vbox ), bouton,
		       FALSE, FALSE, 5 );
  /* Remove button */
  bouton_supprimer_banque = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
  gtk_widget_set_sensitive ( bouton_supprimer_banque, FALSE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_supprimer_banque ), "clicked",
		       GTK_SIGNAL_FUNC  ( supprime_banque ),
		       clist_banques_parametres );
  gtk_box_pack_start ( GTK_BOX ( vbox ), bouton_supprimer_banque,
		       FALSE, FALSE, 5 );



  /* Add a scroll because bank details are huge */
  scrolled_window = gtk_scrolled_window_new ( FALSE, FALSE );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_NEVER,
				   GTK_POLICY_AUTOMATIC );
  gtk_box_pack_start ( GTK_BOX ( vbox_pref ), scrolled_window,
		       TRUE, TRUE, 5 );
  vbox2 = gtk_vbox_new ( FALSE, 0 );
  gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( scrolled_window ),
					  vbox2 );
  gtk_scrolled_window_set_shadow_type ( GTK_SCROLLED_WINDOW ( scrolled_window ),
					GTK_SHADOW_NONE );
  gtk_viewport_set_shadow_type ( GTK_VIEWPORT ( GTK_BIN ( scrolled_window ) -> child ),
                                 GTK_SHADOW_NONE );

  /* Bank details */
  paddingbox = paddingbox_new_with_title ( vbox2, FALSE,
					   _("Bank details") );
  gtk_box_set_child_packing (GTK_BOX(vbox_pref), paddingbox, 
			     FALSE, FALSE, 5, GTK_PACK_START);

  /* Active only if a bank is selected */
  gtk_signal_connect ( GTK_OBJECT ( clist_banques_parametres ),
		       "select-row",
		       GTK_SIGNAL_FUNC ( selection_ligne_banque ),
		       paddingbox );
  gtk_signal_connect ( GTK_OBJECT ( clist_banques_parametres ),
		       "unselect-row",
		       GTK_SIGNAL_FUNC ( deselection_ligne_banque ),
		       paddingbox );

  /* Create a table to align things nicely */
  table = gtk_table_new ( 2, 2, FALSE );
  gtk_table_set_col_spacings ( GTK_TABLE ( table ), 5 );
  gtk_table_set_row_spacings ( GTK_TABLE ( table ), 5 );
  gtk_box_pack_start ( GTK_BOX ( paddingbox ), table,
		       TRUE, TRUE, 5 );

  /* Bank name item */
  label = gtk_label_new ( COLON(_("Name")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 0, 1,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  nom_banque = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     nom_banque, 1, 2, 0, 1,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Bank Sort code item */
  label = gtk_label_new ( COLON(_("Sort code")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 1, 2,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  code_banque = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     code_banque, 1, 2, 1, 2,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Bank address */
  label = gtk_label_new ( COLON(_("Address")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 2, 3,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  code_banque = gtk_entry_new ();
  scrolled_window = gtk_scrolled_window_new ( FALSE, FALSE );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC );
  gtk_widget_show ( scrolled_window );
  adr_banque = gtk_text_new ( FALSE, FALSE );
  gtk_widget_set_usize ( adr_banque, FALSE, 50 );
  gtk_editable_set_editable ( GTK_EDITABLE ( adr_banque ), TRUE );
  gtk_container_add ( GTK_CONTAINER ( scrolled_window ), adr_banque );
  gtk_table_attach ( GTK_TABLE ( table ),
		     scrolled_window, 1, 2, 2, 3,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );


  /* E-mail */
  label = gtk_label_new ( COLON(_("E-Mail")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 3, 4,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  email_banque = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     email_banque, 1, 2, 3, 4,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Phone number */
  label = gtk_label_new ( COLON(_("Phone")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 4, 5,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  tel_banque = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     tel_banque, 1, 2, 4, 5,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Website */
  label = gtk_label_new ( COLON(_("Website")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 5, 6,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  web_banque = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     web_banque, 1, 2, 5, 6,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );


  /* Contact */
  paddingbox = paddingbox_new_with_title ( vbox2, FALSE, _("Contact") );
  table = gtk_table_new ( 2, 2, FALSE );
  gtk_table_set_col_spacings ( GTK_TABLE ( table ), 5 );
  gtk_table_set_row_spacings ( GTK_TABLE ( table ), 5 );
  gtk_box_pack_start ( GTK_BOX ( paddingbox ), table,
		       TRUE, TRUE, 5 );

  label = gtk_label_new ( COLON(_("Name")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 0, 1,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  nom_correspondant = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     nom_correspondant, 1, 2, 0, 1,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Phone number */
  label = gtk_label_new ( COLON(_("Phone number")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 1, 2,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  tel_correspondant = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     tel_correspondant, 1, 2, 1, 2,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* E-Mail */
  label = gtk_label_new ( COLON(_("E-Mail")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 2, 3,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  email_correspondant = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     email_correspondant, 1, 2, 2, 3,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Fax */
  label = gtk_label_new ( COLON(_("Fax")) );
  gtk_misc_set_alignment (GTK_MISC (label), 0, 1);
  gtk_label_set_justify ( GTK_LABEL(label), GTK_JUSTIFY_RIGHT );
  gtk_table_attach ( GTK_TABLE ( table ),
		     label, 0, 1, 3, 4,
		     GTK_SHRINK | GTK_FILL, 0,
		     0, 0 );
  fax_correspondant = gtk_entry_new ();
  gtk_table_attach ( GTK_TABLE ( table ),
		     fax_correspondant, 1, 2, 3, 4,
		     GTK_EXPAND | GTK_FILL, 0,
		     0, 0 );

  /* Notes */
  paddingbox = paddingbox_new_with_title ( vbox2, FALSE, _("Notes") );
  scrolled_window = gtk_scrolled_window_new ( FALSE,
					      FALSE );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC );
  gtk_box_pack_start ( GTK_BOX ( paddingbox ), scrolled_window,
		       TRUE, TRUE, 5 );
  gtk_widget_show ( scrolled_window );
  remarque_banque = gtk_text_new ( FALSE, FALSE );
  gtk_widget_set_usize ( remarque_banque, FALSE, 100 );
  gtk_editable_set_editable ( GTK_EDITABLE ( remarque_banque ), TRUE );
  gtk_container_add ( GTK_CONTAINER ( scrolled_window ), remarque_banque );


/* ajout des bouton appliquer et annuler */

/*   hbox_boutons_modif_banque = gtk_hbox_new ( FALSE, 0 ); */
/*   gtk_widget_set_sensitive ( hbox_boutons_modif_banque, */
/* 			     FALSE ); */
/*   gtk_box_pack_start ( GTK_BOX ( vbox_pref ), */
/* 		       hbox_boutons_modif_banque, */
/* 		       FALSE, */
/* 		       FALSE, */
/* 		       5 ); */
/*   gtk_widget_show ( hbox_boutons_modif_banque ); */

/*   on met ici toutes les connections qui, pour chaque changement, rendent sensitif les boutons */

  /* FIXME: trap them to bank modification */
/*   gtk_signal_connect ( GTK_OBJECT ( nom_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( code_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( tel_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( adr_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*    gtk_signal_connect ( GTK_OBJECT ( email_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( web_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( remarque_banque ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( nom_correspondant ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( fax_correspondant ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( tel_correspondant ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
/*   gtk_signal_connect ( GTK_OBJECT ( email_correspondant ), */
/* 		       "changed", */
/* 		       GTK_SIGNAL_FUNC ( modif_detail_banque ), */
/* 		       NULL); */
  
/*   bouton = gtk_button_new_from_stock (GTK_STOCK_CANCEL); */
/*   //bouton = gnome_stock_button ( GNOME_STOCK_BUTTON_CANCEL ); */
/*   gtk_signal_connect ( GTK_OBJECT ( bouton ), */
/* 		       "clicked", */
/* 		       GTK_SIGNAL_FUNC ( annuler_modif_banque ), */
/* 		       clist_banques_parametres ); */
/*   gtk_box_pack_end ( GTK_BOX ( hbox_boutons_modif_banque ), */
/* 		       bouton, */
/* 		       FALSE, */
/* 		       FALSE, */
/* 		       5 ); */
/*   gtk_widget_show ( bouton ); */

/*   bouton = gtk_button_new_from_stock (GTK_STOCK_APPLY); */
/*   //bouton = gnome_stock_button ( GNOME_STOCK_BUTTON_APPLY ); */
/*   gtk_signal_connect_object ( GTK_OBJECT ( bouton ), */
/* 			      "clicked", */
/* 			      GTK_SIGNAL_FUNC ( applique_modif_banque  ), */
/* 			      GTK_OBJECT ( clist_banques_parametres ) ); */
/*   gtk_box_pack_end ( GTK_BOX ( hbox_boutons_modif_banque ), */
/* 		       bouton, */
/* 		       FALSE, */
/* 		       FALSE, */
/* 		       5 ); */
/*   gtk_widget_show ( bouton ); */


  return ( vbox_pref );

}
/* ************************************************************************************************************** */



/* **************************************************************************************************************************** */
/* Fonction selection_ligne_banque */
/* appel�e lorsqu'on s�lectionne une banque dans la liste */
/* **************************************************************************************************************************** */

void selection_ligne_banque ( GtkWidget *liste,
			      gint ligne,
			      gint colonne,
			      GdkEventButton *ev,
			      GtkWidget *frame )
{
  struct struct_banque *banque;

  ligne_selection_banque = ligne;

  banque = gtk_clist_get_row_data ( GTK_CLIST ( liste ),
				    ligne );

/* remplit tous les champs */

  gtk_entry_set_text ( GTK_ENTRY ( nom_banque ),
		       banque -> nom_banque );

  if ( banque -> code_banque )
    gtk_entry_set_text ( GTK_ENTRY ( code_banque ),
			 banque -> code_banque );

  if ( banque -> tel_banque )
    gtk_entry_set_text ( GTK_ENTRY ( tel_banque ),
			 banque -> tel_banque );

  if ( banque -> email_banque )
    gtk_entry_set_text ( GTK_ENTRY ( email_banque ),
			 banque -> email_banque  );

  if ( banque -> web_banque )
    gtk_entry_set_text ( GTK_ENTRY ( web_banque ),
			 banque -> web_banque );

  if ( banque -> nom_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( nom_correspondant ),
			 banque -> nom_correspondant );

  if ( banque -> tel_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( tel_correspondant ),
			 banque -> tel_correspondant );

  if ( banque -> email_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( email_correspondant ),
			 banque -> email_correspondant );

  if ( banque -> fax_correspondant )
    gtk_entry_set_text ( GTK_ENTRY ( fax_correspondant ),
			 banque -> fax_correspondant );

  if ( banque -> adr_banque )
    gtk_text_insert ( GTK_TEXT ( adr_banque ),
		      NULL,
		      NULL,
		      NULL,
		      banque -> adr_banque,
		      -1 );

  if ( banque -> remarque_banque )
    gtk_text_insert ( GTK_TEXT ( remarque_banque ),
		      NULL,
		      NULL,
		      NULL,
		      banque -> remarque_banque,
		      -1 );

  gtk_widget_set_sensitive ( frame,
			     TRUE );
  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     FALSE );
  gtk_widget_set_sensitive ( bouton_supprimer_banque,
			     TRUE );



}
/* **************************************************************************************************************************** */




/* **************************************************************************************************************************** */
/* Fonction deselection_ligne_banque */
/* appel�e lorsqu'on d�s�lectionne une banque dans la liste */
/* **************************************************************************************************************************** */

void deselection_ligne_banque ( GtkWidget *liste,
				gint ligne,
				gint colonne,
				GdkEventButton *ev,
				GtkWidget *frame )
{
  struct struct_banque *banque;

  if ( GTK_WIDGET_SENSITIVE ( hbox_boutons_modif_banque ) )
    {
      GtkWidget *dialogue;
      GtkWidget *label;
      gint resultat;

      banque = gtk_clist_get_row_data ( GTK_CLIST ( liste ),
					ligne );

      dialogue = gnome_dialog_new ( _("Confirmation of changes"),
				    GNOME_STOCK_BUTTON_YES,
				    GNOME_STOCK_BUTTON_NO,
				    NULL );
      gtk_window_set_transient_for ( GTK_WINDOW ( dialogue ),
				     GTK_WINDOW ( fenetre_preferences ));

      label = gtk_label_new ( g_strconcat ( COLON(_("The bank has changed")),
					    banque -> nom_banque,
					    _("\n\nDo you want to save your changes?"),
					    NULL ) );
      gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialogue ) -> vbox ),
			   label,
			   FALSE,
			   FALSE,
			   0 );
      gtk_widget_show ( label );

      resultat = gnome_dialog_run_and_close ( GNOME_DIALOG ( dialogue ));

      /*       si on veut les enregistrer , ok */
      /* si on ne veut pas et que c'est une nouvelle banque -> on l'efface */

      if ( !resultat )
	applique_modif_banque ( liste );
      else
	{
	  if ( banque -> no_banque == -1 )
	    {
	      gtk_clist_remove ( GTK_CLIST ( liste ),
				 ligne );
	      liste_struct_banques_tmp = g_slist_remove ( liste_struct_banques_tmp,
							  banque );
	      free ( banque );
	    }
	}
    }

/* efface toutes les entr�es */

  gtk_entry_set_text ( GTK_ENTRY ( nom_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( code_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( adr_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( tel_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( email_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( web_banque ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( nom_correspondant ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( tel_correspondant ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( fax_correspondant ),
		       "" );
  gtk_entry_set_text ( GTK_ENTRY ( email_correspondant ),
		       "" );
  gtk_editable_delete_text ( GTK_EDITABLE ( adr_banque ),
			     0,
			     -1 );
  gtk_editable_delete_text ( GTK_EDITABLE ( remarque_banque ),
			     0,
			     -1 );

  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     FALSE );
  gtk_widget_set_sensitive ( frame,
			     FALSE );
  gtk_widget_set_sensitive ( bouton_supprimer_banque,
			     FALSE );


}
/* **************************************************************************************************************************** */



/* **************************************************************************************************************************** */
void modif_detail_banque ( GtkWidget *entree,
			   gpointer null )
{

  gtk_widget_set_sensitive ( hbox_boutons_modif_banque,
			     TRUE );

}
/* **************************************************************************************************************************** */

