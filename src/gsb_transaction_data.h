#ifndef _GSB_TRANSACTION_DATA_H
#define _GSB_TRANSACTION_DATA_H (1)


/** \struct
 * describe a transaction 
 * */

typedef struct
{
    /** @name general stuff */
    gint transaction_number;
    gchar *transaction_id;				/**< filled by ofx */
    gint account_number;
    gdouble transaction_amount;
    gint party_number;
    gchar *notes;
    gint marked_transaction;				/**<  0=nothing, 1=P, 2=T, 3=R */
    gshort automatic_transaction;			/**< 0=manual, 1=automatic (scheduled transaction) */
    gint reconcile_number;
    guint financial_year_number;
    gchar *voucher;
    gchar *bank_references;

    /** @name dates of the transaction */

    GDate *date;
    GDate *value_date;

    /** @name currency stuff */
    gint currency_number;
    gint change_between_account_and_transaction;	/**< if 1 : 1 account_currency = (exchange_rate * amount) transaction_currency */
    gdouble exchange_rate;
    gdouble exchange_fees;

    /** @name category stuff */
    gint category_number;
    gint sub_category_number;
    gint budgetary_number;
    gint sub_budgetary_number;
    gint transaction_number_transfer;
    gint account_number_transfer;			/**< -1 for a deleted account */
    gint breakdown_of_transaction;			/**< 1 if it's a breakdown of transaction */
    gint mother_transaction_number;			/**< for a breakdown, the mother's transaction number */

    /** @name method of payment */
    gint method_of_payment_number;
    gchar *method_of_payment_content;

} struct_transaction;


/* START_DECLARATION */
gint gsb_transaction_data_get_account_number ( gint no_transaction );
gdouble gsb_transaction_data_get_adjusted_amount ( gint no_transaction );
gdouble gsb_transaction_data_get_adjusted_amount_for_currency ( gint no_transaction,
								gint no_currency_for_return );
gdouble gsb_transaction_data_get_amount ( gint no_transaction );
GDate *gsb_transaction_data_get_date ( gint no_transaction );
gint gsb_transaction_data_get_last_number (void);
gchar *gsb_transaction_data_get_transaction_id ( gint no_transaction,
						 gint no_account );
gint gsb_transaction_data_get_transaction_number ( gpointer transaction_pointer );
GDate *gsb_transaction_data_get_value_date ( gint no_transaction );
gboolean gsb_transaction_data_init_variables ( void );
gboolean gsb_transaction_data_set_account_number ( gint no_transaction,
						   gint no_account );
gboolean gsb_transaction_data_set_amount ( gint no_transaction,
					   gdouble amount );
gboolean gsb_transaction_data_set_date ( gint no_transaction,
					 GDate *date );
gboolean gsb_transaction_data_set_transaction_id ( gint no_transaction,
						   gint no_account,
						   gchar *transaction_id );
gboolean gsb_transaction_data_set_transaction_number ( gpointer transaction_pointer,
						       gint no_transaction );
gboolean gsb_transaction_data_set_value_date ( gint no_transaction,
					       GDate *date );
/* END_DECLARATION */


#endif

