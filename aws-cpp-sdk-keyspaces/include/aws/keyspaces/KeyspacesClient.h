﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/keyspaces/Keyspaces_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/keyspaces/KeyspacesServiceClientModel.h>
#include <aws/keyspaces/KeyspacesLegacyAsyncMacros.h>

namespace Aws
{
namespace Keyspaces
{
  /**
   * <p>Amazon Keyspaces (for Apache Cassandra) is a scalable, highly available, and
   * managed Apache Cassandra-compatible database service. Amazon Keyspaces makes it
   * easy to migrate, run, and scale Cassandra workloads in the Amazon Web Services
   * Cloud. With just a few clicks on the Amazon Web Services Management Console or a
   * few lines of code, you can create keyspaces and tables in Amazon Keyspaces,
   * without deploying any infrastructure or installing software. </p> <p>In addition
   * to supporting Cassandra Query Language (CQL) requests via open-source Cassandra
   * drivers, Amazon Keyspaces supports data definition language (DDL) operations to
   * manage keyspaces and tables using the Amazon Web Services SDK and CLI. This API
   * reference describes the supported DDL operations in detail.</p> <p>For the list
   * of all supported CQL APIs, see <a
   * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/cassandra-apis.html">Supported
   * Cassandra APIs, operations, and data types in Amazon Keyspaces</a> in the
   * <i>Amazon Keyspaces Developer Guide</i>.</p> <p>To learn how Amazon Keyspaces
   * API actions are recorded with CloudTrail, see <a
   * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/logging-using-cloudtrail.html#service-name-info-in-cloudtrail">Amazon
   * Keyspaces information in CloudTrail</a> in the <i>Amazon Keyspaces Developer
   * Guide</i>.</p> <p>For more information about Amazon Web Services APIs, for
   * example how to implement retry logic or how to sign Amazon Web Services API
   * requests, see <a
   * href="https://docs.aws.amazon.com/general/latest/gr/aws-apis.html">Amazon Web
   * Services APIs</a> in the <i>General Reference</i>.</p>
   */
  class AWS_KEYSPACES_API KeyspacesClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        KeyspacesClient(const Aws::Keyspaces::KeyspacesClientConfiguration& clientConfiguration = Aws::Keyspaces::KeyspacesClientConfiguration(),
                        std::shared_ptr<KeyspacesEndpointProviderBase> endpointProvider = Aws::MakeShared<KeyspacesEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        KeyspacesClient(const Aws::Auth::AWSCredentials& credentials,
                        std::shared_ptr<KeyspacesEndpointProviderBase> endpointProvider = Aws::MakeShared<KeyspacesEndpointProvider>(ALLOCATION_TAG),
                        const Aws::Keyspaces::KeyspacesClientConfiguration& clientConfiguration = Aws::Keyspaces::KeyspacesClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        KeyspacesClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        std::shared_ptr<KeyspacesEndpointProviderBase> endpointProvider = Aws::MakeShared<KeyspacesEndpointProvider>(ALLOCATION_TAG),
                        const Aws::Keyspaces::KeyspacesClientConfiguration& clientConfiguration = Aws::Keyspaces::KeyspacesClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        KeyspacesClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        KeyspacesClient(const Aws::Auth::AWSCredentials& credentials,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        KeyspacesClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                        const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~KeyspacesClient();


        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         const RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncStreamingOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            const RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableStreamingOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }


        /**
         * <p>The <code>CreateKeyspace</code> operation adds a new keyspace to your
         * account. In an Amazon Web Services account, keyspace names must be unique within
         * each Region.</p> <p> <code>CreateKeyspace</code> is an asynchronous operation.
         * You can monitor the creation status of the new keyspace by using the
         * <code>GetKeyspace</code> operation.</p> <p>For more information, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/working-with-keyspaces.html#keyspaces-create">Creating
         * keyspaces</a> in the <i>Amazon Keyspaces Developer Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/CreateKeyspace">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateKeyspaceOutcome CreateKeyspace(const Model::CreateKeyspaceRequest& request) const;


        /**
         * <p>The <code>CreateTable</code> operation adds a new table to the specified
         * keyspace. Within a keyspace, table names must be unique.</p> <p>
         * <code>CreateTable</code> is an asynchronous operation. When the request is
         * received, the status of the table is set to <code>CREATING</code>. You can
         * monitor the creation status of the new table by using the <code>GetTable</code>
         * operation, which returns the current <code>status</code> of the table. You can
         * start using a table when the status is <code>ACTIVE</code>.</p> <p>For more
         * information, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/working-with-tables.html#tables-create">Creating
         * tables</a> in the <i>Amazon Keyspaces Developer Guide</i>.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/CreateTable">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateTableOutcome CreateTable(const Model::CreateTableRequest& request) const;


        /**
         * <p>The <code>DeleteKeyspace</code> operation deletes a keyspace and all of its
         * tables. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/DeleteKeyspace">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteKeyspaceOutcome DeleteKeyspace(const Model::DeleteKeyspaceRequest& request) const;


        /**
         * <p>The <code>DeleteTable</code> operation deletes a table and all of its data.
         * After a <code>DeleteTable</code> request is received, the specified table is in
         * the <code>DELETING</code> state until Amazon Keyspaces completes the deletion.
         * If the table is in the <code>ACTIVE</code> state, you can delete it. If a table
         * is either in the <code>CREATING</code> or <code>UPDATING</code> states, then
         * Amazon Keyspaces returns a <code>ResourceInUseException</code>. If the specified
         * table does not exist, Amazon Keyspaces returns a
         * <code>ResourceNotFoundException</code>. If the table is already in the
         * <code>DELETING</code> state, no error is returned.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/DeleteTable">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteTableOutcome DeleteTable(const Model::DeleteTableRequest& request) const;


        /**
         * <p>Returns the name and the Amazon Resource Name (ARN) of the specified
         * table.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/GetKeyspace">AWS
         * API Reference</a></p>
         */
        virtual Model::GetKeyspaceOutcome GetKeyspace(const Model::GetKeyspaceRequest& request) const;


        /**
         * <p>Returns information about the table, including the table's name and current
         * status, the keyspace name, configuration settings, and metadata.</p> <p>To read
         * table metadata using <code>GetTable</code>, <code>Select</code> action
         * permissions for the table and system tables are required to complete the
         * operation.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/GetTable">AWS
         * API Reference</a></p>
         */
        virtual Model::GetTableOutcome GetTable(const Model::GetTableRequest& request) const;


        /**
         * <p>Returns a list of keyspaces.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/ListKeyspaces">AWS
         * API Reference</a></p>
         */
        virtual Model::ListKeyspacesOutcome ListKeyspaces(const Model::ListKeyspacesRequest& request) const;


        /**
         * <p>Returns a list of tables for a specified keyspace.</p><p><h3>See Also:</h3>  
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/ListTables">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTablesOutcome ListTables(const Model::ListTablesRequest& request) const;


        /**
         * <p>Returns a list of all tags associated with the specified Amazon Keyspaces
         * resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Restores the specified table to the specified point in time within the
         * <code>earliest_restorable_timestamp</code> and the current time. For more
         * information about restore points, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/PointInTimeRecovery_HowItWorks.html#howitworks_backup_window">
         * Time window for PITR continuous backups</a> in the <i>Amazon Keyspaces Developer
         * Guide</i>. </p> <p>Any number of users can execute up to 4 concurrent restores
         * (any type of restore) in a given account.</p> <p>When you restore using point in
         * time recovery, Amazon Keyspaces restores your source table's schema and data to
         * the state based on the selected timestamp <code>(day:hour:minute:second)</code>
         * to a new table. The Time to Live (TTL) settings are also restored to the state
         * based on the selected timestamp.</p> <p>In addition to the table's schema, data,
         * and TTL settings, <code>RestoreTable</code> restores the capacity mode,
         * encryption, and point-in-time recovery settings from the source table. Unlike
         * the table's schema data and TTL settings, which are restored based on the
         * selected timestamp, these settings are always restored based on the table's
         * settings as of the current time or when the table was deleted.</p> <p>You can
         * also overwrite these settings during restore:</p> <p>• Read/write capacity
         * mode</p> <p>• Provisioned throughput capacity settings</p> <p>• Point-in-time
         * (PITR) settings</p> <p>• Tags</p> <p>For more information, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/PointInTimeRecovery_HowItWorks.html#howitworks_backup_settings">PITR
         * restore settings</a> in the <i>Amazon Keyspaces Developer Guide</i>.</p> <p>Note
         * that the following settings are not restored, and you must configure them
         * manually for the new table:</p> <p>• Automatic scaling policies (for tables that
         * use provisioned capacity mode)</p> <p>• Identity and Access Management (IAM)
         * policies</p> <p>• Amazon CloudWatch metrics and alarms</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/RestoreTable">AWS
         * API Reference</a></p>
         */
        virtual Model::RestoreTableOutcome RestoreTable(const Model::RestoreTableRequest& request) const;


        /**
         * <p>Associates a set of tags with a Amazon Keyspaces resource. You can then
         * activate these user-defined tags so that they appear on the Cost Management
         * Console for cost allocation tracking. For more information, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/tagging-keyspaces.html">Adding
         * tags and labels to Amazon Keyspaces resources</a> in the <i>Amazon Keyspaces
         * Developer Guide</i>.</p> <p>For IAM policy examples that show how to control
         * access to Amazon Keyspaces resources based on tags, see <a
         * href="https://docs.aws.amazon.com/keyspaces/latest/devguide/security_iam_id-based-policy-examples-tags">Amazon
         * Keyspaces resource access based on tags</a> in the <i>Amazon Keyspaces Developer
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Removes the association of tags from a Amazon Keyspaces
         * resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Adds new columns to the table or updates one of the table's settings, for
         * example capacity mode, encryption, point-in-time recovery, or ttl settings. Note
         * that you can only update one specific table setting per update
         * operation.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/keyspaces-2022-02-10/UpdateTable">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateTableOutcome UpdateTable(const Model::UpdateTableRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<KeyspacesEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const KeyspacesClientConfiguration& clientConfiguration);

      KeyspacesClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<KeyspacesEndpointProviderBase> m_endpointProvider;
  };

} // namespace Keyspaces
} // namespace Aws
