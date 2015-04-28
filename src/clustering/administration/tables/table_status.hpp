// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_TABLES_TABLE_STATUS_HPP_
#define CLUSTERING_ADMINISTRATION_TABLES_TABLE_STATUS_HPP_

#include <string>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "clustering/administration/tables/table_common.hpp"

class server_config_client_t;
class table_status_artificial_table_backend_t;

enum class table_wait_result_t {
    WAITED,    // The table is ready after waiting for it
    IMMEDIATE, // The table was already ready
    DELETED,   // The table has been deleted
};

table_wait_result_t wait_for_table_readiness(
    const namespace_id_t &table_id,
    table_readiness_t readiness,
    const table_status_artificial_table_backend_t *table_status_backend,
    signal_t *interruptor,
    ql::datum_t *status_out);

class table_status_artificial_table_backend_t :
    public common_table_artificial_table_backend_t
{
public:
    table_status_artificial_table_backend_t(
            boost::shared_ptr<semilattice_readwrite_view_t<
                cluster_semilattice_metadata_t> > _semilattice_view,
            table_meta_client_t *_table_meta_client,
            admin_identifier_format_t _identifier_format,
            server_config_client_t *_server_config_client);
    ~table_status_artificial_table_backend_t();

    bool write_row(
            ql::datum_t primary_key,
            bool pkey_was_autogenerated,
            ql::datum_t *new_value_inout,
            signal_t *interruptor,
            std::string *error_out);

private:
    bool format_row(
            namespace_id_t table_id,
            const ql::datum_t &db_name_or_uuid,
            const table_config_and_shards_t &config_and_shards,
            signal_t *interruptor,
            ql::datum_t *row_out,
            std::string *error_out);

    friend table_wait_result_t wait_for_table_readiness(
        const namespace_id_t &,
        table_readiness_t,
        const table_status_artificial_table_backend_t *,
        signal_t *,
        ql::datum_t *);

    server_config_client_t *server_config_client;
};

#endif /* CLUSTERING_ADMINISTRATION_TABLES_TABLE_STATUS_HPP_ */

