﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/dms/DatabaseMigrationService_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/dms/DatabaseMigrationServiceServiceClientModel.h>
#include <aws/dms/DatabaseMigrationServiceLegacyAsyncMacros.h>

namespace Aws
{
namespace DatabaseMigrationService
{
  /**
   * <fullname>Database Migration Service</fullname> <p>Database Migration Service
   * (DMS) can migrate your data to and from the most widely used commercial and
   * open-source databases such as Oracle, PostgreSQL, Microsoft SQL Server, Amazon
   * Redshift, MariaDB, Amazon Aurora, MySQL, and SAP Adaptive Server Enterprise
   * (ASE). The service supports homogeneous migrations such as Oracle to Oracle, as
   * well as heterogeneous migrations between different database platforms, such as
   * Oracle to MySQL or SQL Server to PostgreSQL.</p> <p>For more information about
   * DMS, see <a
   * href="https://docs.aws.amazon.com/dms/latest/userguide/Welcome.html">What Is
   * Database Migration Service?</a> in the <i>Database Migration Service User
   * Guide.</i> </p>
   */
  class AWS_DATABASEMIGRATIONSERVICE_API DatabaseMigrationServiceClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        DatabaseMigrationServiceClient(const Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration& clientConfiguration = Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration(),
                                       std::shared_ptr<DatabaseMigrationServiceEndpointProviderBase> endpointProvider = Aws::MakeShared<DatabaseMigrationServiceEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        DatabaseMigrationServiceClient(const Aws::Auth::AWSCredentials& credentials,
                                       std::shared_ptr<DatabaseMigrationServiceEndpointProviderBase> endpointProvider = Aws::MakeShared<DatabaseMigrationServiceEndpointProvider>(ALLOCATION_TAG),
                                       const Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration& clientConfiguration = Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        DatabaseMigrationServiceClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                       std::shared_ptr<DatabaseMigrationServiceEndpointProviderBase> endpointProvider = Aws::MakeShared<DatabaseMigrationServiceEndpointProvider>(ALLOCATION_TAG),
                                       const Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration& clientConfiguration = Aws::DatabaseMigrationService::DatabaseMigrationServiceClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        DatabaseMigrationServiceClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        DatabaseMigrationServiceClient(const Aws::Auth::AWSCredentials& credentials,
                                       const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        DatabaseMigrationServiceClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                       const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~DatabaseMigrationServiceClient();


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
         * <p>Adds metadata tags to an DMS resource, including replication instance,
         * endpoint, security group, and migration task. These tags can also be used with
         * cost allocation reporting to track cost associated with DMS resources, or used
         * in a Condition statement in an IAM policy for DMS. For more information, see <a
         * href="https://docs.aws.amazon.com/dms/latest/APIReference/API_Tag.html">
         * <code>Tag</code> </a> data type description.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/AddTagsToResource">AWS
         * API Reference</a></p>
         */
        virtual Model::AddTagsToResourceOutcome AddTagsToResource(const Model::AddTagsToResourceRequest& request) const;


        /**
         * <p>Applies a pending maintenance action to a resource (for example, to a
         * replication instance).</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ApplyPendingMaintenanceAction">AWS
         * API Reference</a></p>
         */
        virtual Model::ApplyPendingMaintenanceActionOutcome ApplyPendingMaintenanceAction(const Model::ApplyPendingMaintenanceActionRequest& request) const;


        /**
         * <p>Cancels a single premigration assessment run.</p> <p>This operation prevents
         * any individual assessments from running if they haven't started running. It also
         * attempts to cancel any individual assessments that are currently
         * running.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CancelReplicationTaskAssessmentRun">AWS
         * API Reference</a></p>
         */
        virtual Model::CancelReplicationTaskAssessmentRunOutcome CancelReplicationTaskAssessmentRun(const Model::CancelReplicationTaskAssessmentRunRequest& request) const;


        /**
         * <p>Creates an endpoint using the provided settings.</p>  <p>For a MySQL
         * source or target endpoint, don't explicitly specify the database using the
         * <code>DatabaseName</code> request parameter on the <code>CreateEndpoint</code>
         * API call. Specifying <code>DatabaseName</code> when you create a MySQL endpoint
         * replicates all the task tables to this single database. For MySQL endpoints, you
         * specify the database only when you specify the schema in the table-mapping rules
         * of the DMS task.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEndpointOutcome CreateEndpoint(const Model::CreateEndpointRequest& request) const;


        /**
         * <p> Creates an DMS event notification subscription. </p> <p>You can specify the
         * type of source (<code>SourceType</code>) you want to be notified of, provide a
         * list of DMS source IDs (<code>SourceIds</code>) that triggers the events, and
         * provide a list of event categories (<code>EventCategories</code>) for events you
         * want to be notified of. If you specify both the <code>SourceType</code> and
         * <code>SourceIds</code>, such as <code>SourceType = replication-instance</code>
         * and <code>SourceIdentifier = my-replinstance</code>, you will be notified of all
         * the replication instance events for the specified source. If you specify a
         * <code>SourceType</code> but don't specify a <code>SourceIdentifier</code>, you
         * receive notice of the events for that source type for all your DMS sources. If
         * you don't specify either <code>SourceType</code> nor
         * <code>SourceIdentifier</code>, you will be notified of events generated from all
         * DMS sources belonging to your customer account.</p> <p>For more information
         * about DMS events, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Events.html">Working
         * with Events and Notifications</a> in the <i>Database Migration Service User
         * Guide.</i> </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateEventSubscription">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateEventSubscriptionOutcome CreateEventSubscription(const Model::CreateEventSubscriptionRequest& request) const;


        /**
         * <p>Creates a Fleet Advisor collector using the specified
         * parameters.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateFleetAdvisorCollector">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateFleetAdvisorCollectorOutcome CreateFleetAdvisorCollector(const Model::CreateFleetAdvisorCollectorRequest& request) const;


        /**
         * <p>Creates the replication instance using the specified parameters.</p> <p>DMS
         * requires that your account have certain roles with appropriate permissions
         * before you can create a replication instance. For information on the required
         * roles, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Security.html#CHAP_Security.APIRole">Creating
         * the IAM Roles to Use With the CLI and DMS API</a>. For information on the
         * required permissions, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Security.html#CHAP_Security.IAMPermissions">IAM
         * Permissions Needed to Use DMS</a>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateReplicationInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReplicationInstanceOutcome CreateReplicationInstance(const Model::CreateReplicationInstanceRequest& request) const;


        /**
         * <p>Creates a replication subnet group given a list of the subnet IDs in a
         * VPC.</p> <p>The VPC needs to have at least one subnet in at least two
         * availability zones in the Amazon Web Services Region, otherwise the service will
         * throw a <code>ReplicationSubnetGroupDoesNotCoverEnoughAZs</code>
         * exception.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateReplicationSubnetGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReplicationSubnetGroupOutcome CreateReplicationSubnetGroup(const Model::CreateReplicationSubnetGroupRequest& request) const;


        /**
         * <p>Creates a replication task using the specified parameters.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/CreateReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateReplicationTaskOutcome CreateReplicationTask(const Model::CreateReplicationTaskRequest& request) const;


        /**
         * <p>Deletes the specified certificate. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteCertificate">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteCertificateOutcome DeleteCertificate(const Model::DeleteCertificateRequest& request) const;


        /**
         * <p>Deletes the connection between a replication instance and an
         * endpoint.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConnectionOutcome DeleteConnection(const Model::DeleteConnectionRequest& request) const;


        /**
         * <p>Deletes the specified endpoint.</p>  <p>All tasks associated with the
         * endpoint must be deleted before you can delete the endpoint.</p> 
         * <p/><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEndpointOutcome DeleteEndpoint(const Model::DeleteEndpointRequest& request) const;


        /**
         * <p> Deletes an DMS event subscription. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteEventSubscription">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteEventSubscriptionOutcome DeleteEventSubscription(const Model::DeleteEventSubscriptionRequest& request) const;


        /**
         * <p>Deletes the specified Fleet Advisor collector.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteFleetAdvisorCollector">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFleetAdvisorCollectorOutcome DeleteFleetAdvisorCollector(const Model::DeleteFleetAdvisorCollectorRequest& request) const;


        /**
         * <p>Deletes the specified Fleet Advisor collector databases.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteFleetAdvisorDatabases">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteFleetAdvisorDatabasesOutcome DeleteFleetAdvisorDatabases(const Model::DeleteFleetAdvisorDatabasesRequest& request) const;


        /**
         * <p>Deletes the specified replication instance.</p>  <p>You must delete any
         * migration tasks that are associated with the replication instance before you can
         * delete it.</p>  <p/><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteReplicationInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReplicationInstanceOutcome DeleteReplicationInstance(const Model::DeleteReplicationInstanceRequest& request) const;


        /**
         * <p>Deletes a subnet group.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteReplicationSubnetGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReplicationSubnetGroupOutcome DeleteReplicationSubnetGroup(const Model::DeleteReplicationSubnetGroupRequest& request) const;


        /**
         * <p>Deletes the specified replication task.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReplicationTaskOutcome DeleteReplicationTask(const Model::DeleteReplicationTaskRequest& request) const;


        /**
         * <p>Deletes the record of a single premigration assessment run.</p> <p>This
         * operation removes all metadata that DMS maintains about this assessment run.
         * However, the operation leaves untouched all information about this assessment
         * run that is stored in your Amazon S3 bucket.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DeleteReplicationTaskAssessmentRun">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteReplicationTaskAssessmentRunOutcome DeleteReplicationTaskAssessmentRun(const Model::DeleteReplicationTaskAssessmentRunRequest& request) const;


        /**
         * <p>Lists all of the DMS attributes for a customer account. These attributes
         * include DMS quotas for the account and a unique account identifier in a
         * particular DMS region. DMS quotas include a list of resource quotas supported by
         * the account, such as the number of replication instances allowed. The
         * description for each resource quota, includes the quota name, current usage
         * toward that quota, and the quota's maximum value. DMS uses the unique account
         * identifier to name each artifact used by DMS in the given region.</p> <p>This
         * command does not take any parameters.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeAccountAttributes">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeAccountAttributesOutcome DescribeAccountAttributes(const Model::DescribeAccountAttributesRequest& request) const;


        /**
         * <p>Provides a list of individual assessments that you can specify for a new
         * premigration assessment run, given one or more parameters.</p> <p>If you specify
         * an existing migration task, this operation provides the default individual
         * assessments you can specify for that task. Otherwise, the specified parameters
         * model elements of a possible migration task on which to base a premigration
         * assessment run.</p> <p>To use these migration task modeling parameters, you must
         * specify an existing replication instance, a source database engine, a target
         * database engine, and a migration type. This combination of parameters
         * potentially limits the default individual assessments available for an
         * assessment run created for a corresponding migration task.</p> <p>If you specify
         * no parameters, this operation provides a list of all possible individual
         * assessments that you can specify for an assessment run. If you specify any one
         * of the task modeling parameters, you must specify all of them or the operation
         * cannot provide a list of individual assessments. The only parameter that you can
         * specify alone is for an existing migration task. The specified task definition
         * then determines the default list of individual assessments that you can specify
         * in an assessment run for the task.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeApplicableIndividualAssessments">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeApplicableIndividualAssessmentsOutcome DescribeApplicableIndividualAssessments(const Model::DescribeApplicableIndividualAssessmentsRequest& request) const;


        /**
         * <p>Provides a description of the certificate.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeCertificates">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeCertificatesOutcome DescribeCertificates(const Model::DescribeCertificatesRequest& request) const;


        /**
         * <p>Describes the status of the connections that have been made between the
         * replication instance and an endpoint. Connections are created when you test an
         * endpoint.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeConnections">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeConnectionsOutcome DescribeConnections(const Model::DescribeConnectionsRequest& request) const;


        /**
         * <p>Returns information about the possible endpoint settings available when you
         * create an endpoint for a specific database engine.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEndpointSettings">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEndpointSettingsOutcome DescribeEndpointSettings(const Model::DescribeEndpointSettingsRequest& request) const;


        /**
         * <p>Returns information about the type of endpoints available.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEndpointTypes">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEndpointTypesOutcome DescribeEndpointTypes(const Model::DescribeEndpointTypesRequest& request) const;


        /**
         * <p>Returns information about the endpoints for your account in the current
         * region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEndpoints">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEndpointsOutcome DescribeEndpoints(const Model::DescribeEndpointsRequest& request) const;


        /**
         * <p>Lists categories for all event source types, or, if specified, for a
         * specified source type. You can see a list of the event categories and source
         * types in <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Events.html">Working
         * with Events and Notifications</a> in the <i>Database Migration Service User
         * Guide.</i> </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEventCategories">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEventCategoriesOutcome DescribeEventCategories(const Model::DescribeEventCategoriesRequest& request) const;


        /**
         * <p>Lists all the event subscriptions for a customer account. The description of
         * a subscription includes <code>SubscriptionName</code>, <code>SNSTopicARN</code>,
         * <code>CustomerID</code>, <code>SourceType</code>, <code>SourceID</code>,
         * <code>CreationTime</code>, and <code>Status</code>. </p> <p>If you specify
         * <code>SubscriptionName</code>, this action lists the description for that
         * subscription.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEventSubscriptions">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEventSubscriptionsOutcome DescribeEventSubscriptions(const Model::DescribeEventSubscriptionsRequest& request) const;


        /**
         * <p> Lists events for a given source identifier and source type. You can also
         * specify a start and end time. For more information on DMS events, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Events.html">Working
         * with Events and Notifications</a> in the <i>Database Migration Service User
         * Guide.</i> </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeEvents">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeEventsOutcome DescribeEvents(const Model::DescribeEventsRequest& request) const;


        /**
         * <p>Returns a list of the Fleet Advisor collectors in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeFleetAdvisorCollectors">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFleetAdvisorCollectorsOutcome DescribeFleetAdvisorCollectors(const Model::DescribeFleetAdvisorCollectorsRequest& request) const;


        /**
         * <p>Returns a list of Fleet Advisor databases in your account.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeFleetAdvisorDatabases">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFleetAdvisorDatabasesOutcome DescribeFleetAdvisorDatabases(const Model::DescribeFleetAdvisorDatabasesRequest& request) const;


        /**
         * <p>Provides descriptions of large-scale assessment (LSA) analyses produced by
         * your Fleet Advisor collectors. </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeFleetAdvisorLsaAnalysis">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFleetAdvisorLsaAnalysisOutcome DescribeFleetAdvisorLsaAnalysis(const Model::DescribeFleetAdvisorLsaAnalysisRequest& request) const;


        /**
         * <p>Provides descriptions of the schemas discovered by your Fleet Advisor
         * collectors.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeFleetAdvisorSchemaObjectSummary">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFleetAdvisorSchemaObjectSummaryOutcome DescribeFleetAdvisorSchemaObjectSummary(const Model::DescribeFleetAdvisorSchemaObjectSummaryRequest& request) const;


        /**
         * <p>Returns a list of schemas detected by Fleet Advisor Collectors in your
         * account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeFleetAdvisorSchemas">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeFleetAdvisorSchemasOutcome DescribeFleetAdvisorSchemas(const Model::DescribeFleetAdvisorSchemasRequest& request) const;


        /**
         * <p>Returns information about the replication instance types that can be created
         * in the specified region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeOrderableReplicationInstances">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeOrderableReplicationInstancesOutcome DescribeOrderableReplicationInstances(const Model::DescribeOrderableReplicationInstancesRequest& request) const;


        /**
         * <p>For internal use only</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribePendingMaintenanceActions">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribePendingMaintenanceActionsOutcome DescribePendingMaintenanceActions(const Model::DescribePendingMaintenanceActionsRequest& request) const;


        /**
         * <p>Returns the status of the RefreshSchemas operation.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeRefreshSchemasStatus">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeRefreshSchemasStatusOutcome DescribeRefreshSchemasStatus(const Model::DescribeRefreshSchemasStatusRequest& request) const;


        /**
         * <p>Returns information about the task logs for the specified task.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationInstanceTaskLogs">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationInstanceTaskLogsOutcome DescribeReplicationInstanceTaskLogs(const Model::DescribeReplicationInstanceTaskLogsRequest& request) const;


        /**
         * <p>Returns information about replication instances for your account in the
         * current region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationInstances">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationInstancesOutcome DescribeReplicationInstances(const Model::DescribeReplicationInstancesRequest& request) const;


        /**
         * <p>Returns information about the replication subnet groups.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationSubnetGroups">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationSubnetGroupsOutcome DescribeReplicationSubnetGroups(const Model::DescribeReplicationSubnetGroupsRequest& request) const;


        /**
         * <p>Returns the task assessment results from the Amazon S3 bucket that DMS
         * creates in your Amazon Web Services account. This action always returns the
         * latest results.</p> <p>For more information about DMS task assessments, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Tasks.AssessmentReport.html">Creating
         * a task assessment report</a> in the <i>Database Migration Service User
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationTaskAssessmentResults">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationTaskAssessmentResultsOutcome DescribeReplicationTaskAssessmentResults(const Model::DescribeReplicationTaskAssessmentResultsRequest& request) const;


        /**
         * <p>Returns a paginated list of premigration assessment runs based on filter
         * settings.</p> <p>These filter settings can specify a combination of premigration
         * assessment runs, migration tasks, replication instances, and assessment run
         * status values.</p>  <p>This operation doesn't return information about
         * individual assessments. For this information, see the
         * <code>DescribeReplicationTaskIndividualAssessments</code> operation. </p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationTaskAssessmentRuns">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationTaskAssessmentRunsOutcome DescribeReplicationTaskAssessmentRuns(const Model::DescribeReplicationTaskAssessmentRunsRequest& request) const;


        /**
         * <p>Returns a paginated list of individual assessments based on filter
         * settings.</p> <p>These filter settings can specify a combination of premigration
         * assessment runs, migration tasks, and assessment status values.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationTaskIndividualAssessments">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationTaskIndividualAssessmentsOutcome DescribeReplicationTaskIndividualAssessments(const Model::DescribeReplicationTaskIndividualAssessmentsRequest& request) const;


        /**
         * <p>Returns information about replication tasks for your account in the current
         * region.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeReplicationTasks">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeReplicationTasksOutcome DescribeReplicationTasks(const Model::DescribeReplicationTasksRequest& request) const;


        /**
         * <p>Returns information about the schema for the specified endpoint.</p>
         * <p/><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeSchemas">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeSchemasOutcome DescribeSchemas(const Model::DescribeSchemasRequest& request) const;


        /**
         * <p>Returns table statistics on the database migration task, including table
         * name, rows inserted, rows updated, and rows deleted.</p> <p>Note that the "last
         * updated" column the DMS console only indicates the time that DMS last updated
         * the table statistics record for a table. It does not indicate the time of the
         * last update to the table.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/DescribeTableStatistics">AWS
         * API Reference</a></p>
         */
        virtual Model::DescribeTableStatisticsOutcome DescribeTableStatistics(const Model::DescribeTableStatisticsRequest& request) const;


        /**
         * <p>Uploads the specified certificate.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ImportCertificate">AWS
         * API Reference</a></p>
         */
        virtual Model::ImportCertificateOutcome ImportCertificate(const Model::ImportCertificateRequest& request) const;


        /**
         * <p>Lists all metadata tags attached to an DMS resource, including replication
         * instance, endpoint, security group, and migration task. For more information,
         * see <a href="https://docs.aws.amazon.com/dms/latest/APIReference/API_Tag.html">
         * <code>Tag</code> </a> data type description.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Modifies the specified endpoint.</p>  <p>For a MySQL source or target
         * endpoint, don't explicitly specify the database using the
         * <code>DatabaseName</code> request parameter on the <code>ModifyEndpoint</code>
         * API call. Specifying <code>DatabaseName</code> when you modify a MySQL endpoint
         * replicates all the task tables to this single database. For MySQL endpoints, you
         * specify the database only when you specify the schema in the table-mapping rules
         * of the DMS task.</p> <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ModifyEndpoint">AWS
         * API Reference</a></p>
         */
        virtual Model::ModifyEndpointOutcome ModifyEndpoint(const Model::ModifyEndpointRequest& request) const;


        /**
         * <p>Modifies an existing DMS event notification subscription. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ModifyEventSubscription">AWS
         * API Reference</a></p>
         */
        virtual Model::ModifyEventSubscriptionOutcome ModifyEventSubscription(const Model::ModifyEventSubscriptionRequest& request) const;


        /**
         * <p>Modifies the replication instance to apply new settings. You can change one
         * or more parameters by specifying these parameters and the new values in the
         * request.</p> <p>Some settings are applied during the maintenance window.</p>
         * <p/><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ModifyReplicationInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::ModifyReplicationInstanceOutcome ModifyReplicationInstance(const Model::ModifyReplicationInstanceRequest& request) const;


        /**
         * <p>Modifies the settings for the specified replication subnet
         * group.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ModifyReplicationSubnetGroup">AWS
         * API Reference</a></p>
         */
        virtual Model::ModifyReplicationSubnetGroupOutcome ModifyReplicationSubnetGroup(const Model::ModifyReplicationSubnetGroupRequest& request) const;


        /**
         * <p>Modifies the specified replication task.</p> <p>You can't modify the task
         * endpoints. The task must be stopped before you can modify it. </p> <p>For more
         * information about DMS tasks, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Tasks.html">Working
         * with Migration Tasks</a> in the <i>Database Migration Service User
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ModifyReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::ModifyReplicationTaskOutcome ModifyReplicationTask(const Model::ModifyReplicationTaskRequest& request) const;


        /**
         * <p>Moves a replication task from its current replication instance to a different
         * target replication instance using the specified parameters. The target
         * replication instance must be created with the same or later DMS version as the
         * current replication instance.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/MoveReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::MoveReplicationTaskOutcome MoveReplicationTask(const Model::MoveReplicationTaskRequest& request) const;


        /**
         * <p>Reboots a replication instance. Rebooting results in a momentary outage,
         * until the replication instance becomes available again.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/RebootReplicationInstance">AWS
         * API Reference</a></p>
         */
        virtual Model::RebootReplicationInstanceOutcome RebootReplicationInstance(const Model::RebootReplicationInstanceRequest& request) const;


        /**
         * <p>Populates the schema for the specified endpoint. This is an asynchronous
         * operation and can take several minutes. You can check the status of this
         * operation by calling the DescribeRefreshSchemasStatus operation.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/RefreshSchemas">AWS
         * API Reference</a></p>
         */
        virtual Model::RefreshSchemasOutcome RefreshSchemas(const Model::RefreshSchemasRequest& request) const;


        /**
         * <p>Reloads the target database table with the source data. </p> <p>You can only
         * use this operation with a task in the <code>RUNNING</code> state, otherwise the
         * service will throw an <code>InvalidResourceStateFault</code>
         * exception.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/ReloadTables">AWS
         * API Reference</a></p>
         */
        virtual Model::ReloadTablesOutcome ReloadTables(const Model::ReloadTablesRequest& request) const;


        /**
         * <p>Removes metadata tags from an DMS resource, including replication instance,
         * endpoint, security group, and migration task. For more information, see <a
         * href="https://docs.aws.amazon.com/dms/latest/APIReference/API_Tag.html">
         * <code>Tag</code> </a> data type description.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/RemoveTagsFromResource">AWS
         * API Reference</a></p>
         */
        virtual Model::RemoveTagsFromResourceOutcome RemoveTagsFromResource(const Model::RemoveTagsFromResourceRequest& request) const;


        /**
         * <p>Runs large-scale assessment (LSA) analysis on every Fleet Advisor collector
         * in your account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/RunFleetAdvisorLsaAnalysis">AWS
         * API Reference</a></p>
         */
        virtual Model::RunFleetAdvisorLsaAnalysisOutcome RunFleetAdvisorLsaAnalysis() const;

        /**
         * A Callable wrapper for RunFleetAdvisorLsaAnalysis that returns a future to the operation so that it can be executed in parallel to other requests.
         */
        virtual Model::RunFleetAdvisorLsaAnalysisOutcomeCallable RunFleetAdvisorLsaAnalysisCallable() const;

        /**
         * An Async wrapper for RunFleetAdvisorLsaAnalysis that queues the request into a thread executor and triggers associated callback when operation has finished.
         */
        virtual void RunFleetAdvisorLsaAnalysisAsync(const RunFleetAdvisorLsaAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context = nullptr) const;
        /**
         * <p>Starts the replication task.</p> <p>For more information about DMS tasks, see
         * <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Tasks.html">Working
         * with Migration Tasks </a> in the <i>Database Migration Service User Guide.</i>
         * </p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/StartReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::StartReplicationTaskOutcome StartReplicationTask(const Model::StartReplicationTaskRequest& request) const;


        /**
         * <p> Starts the replication task assessment for unsupported data types in the
         * source database. </p> <p>You can only use this operation for a task if the
         * following conditions are true:</p> <ul> <li> <p>The task must be in the
         * <code>stopped</code> state.</p> </li> <li> <p>The task must have successful
         * connections to the source and target.</p> </li> </ul> <p>If either of these
         * conditions are not met, an <code>InvalidResourceStateFault</code> error will
         * result. </p> <p>For information about DMS task assessments, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Tasks.AssessmentReport.html">Creating
         * a task assessment report</a> in the <i>Database Migration Service User
         * Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/StartReplicationTaskAssessment">AWS
         * API Reference</a></p>
         */
        virtual Model::StartReplicationTaskAssessmentOutcome StartReplicationTaskAssessment(const Model::StartReplicationTaskAssessmentRequest& request) const;


        /**
         * <p>Starts a new premigration assessment run for one or more individual
         * assessments of a migration task.</p> <p>The assessments that you can specify
         * depend on the source and target database engine and the migration type defined
         * for the given task. To run this operation, your migration task must already be
         * created. After you run this operation, you can review the status of each
         * individual assessment. You can also run the migration task manually after the
         * assessment run and its individual assessments complete.</p><p><h3>See Also:</h3>
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/StartReplicationTaskAssessmentRun">AWS
         * API Reference</a></p>
         */
        virtual Model::StartReplicationTaskAssessmentRunOutcome StartReplicationTaskAssessmentRun(const Model::StartReplicationTaskAssessmentRunRequest& request) const;


        /**
         * <p>Stops the replication task.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/StopReplicationTask">AWS
         * API Reference</a></p>
         */
        virtual Model::StopReplicationTaskOutcome StopReplicationTask(const Model::StopReplicationTaskRequest& request) const;


        /**
         * <p>Tests the connection between the replication instance and the
         * endpoint.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/TestConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::TestConnectionOutcome TestConnection(const Model::TestConnectionRequest& request) const;


        /**
         * <p>Migrates 10 active and enabled Amazon SNS subscriptions at a time and
         * converts them to corresponding Amazon EventBridge rules. By default, this
         * operation migrates subscriptions only when all your replication instance
         * versions are 3.4.6 or higher. If any replication instances are from versions
         * earlier than 3.4.6, the operation raises an error and tells you to upgrade these
         * instances to version 3.4.6 or higher. To enable migration regardless of version,
         * set the <code>Force</code> option to true. However, if you don't upgrade
         * instances earlier than version 3.4.6, some types of events might not be
         * available when you use Amazon EventBridge.</p> <p>To call this operation, make
         * sure that you have certain permissions added to your user account. For more
         * information, see <a
         * href="https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Events.html#CHAP_Events-migrate-to-eventbridge">Migrating
         * event subscriptions to Amazon EventBridge</a> in the <i>Amazon Web Services
         * Database Migration Service User Guide</i>.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/dms-2016-01-01/UpdateSubscriptionsToEventBridge">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateSubscriptionsToEventBridgeOutcome UpdateSubscriptionsToEventBridge(const Model::UpdateSubscriptionsToEventBridgeRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<DatabaseMigrationServiceEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const DatabaseMigrationServiceClientConfiguration& clientConfiguration);

      DatabaseMigrationServiceClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<DatabaseMigrationServiceEndpointProviderBase> m_endpointProvider;
  };

} // namespace DatabaseMigrationService
} // namespace Aws
