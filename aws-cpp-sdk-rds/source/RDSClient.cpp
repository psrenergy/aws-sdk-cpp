﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/xml/XmlSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/rds/RDSClient.h>
#include <aws/rds/RDSErrorMarshaller.h>
#include <aws/rds/RDSEndpointProvider.h>
#include <aws/rds/model/AddRoleToDBClusterRequest.h>
#include <aws/rds/model/AddRoleToDBInstanceRequest.h>
#include <aws/rds/model/AddSourceIdentifierToSubscriptionRequest.h>
#include <aws/rds/model/AddTagsToResourceRequest.h>
#include <aws/rds/model/ApplyPendingMaintenanceActionRequest.h>
#include <aws/rds/model/AuthorizeDBSecurityGroupIngressRequest.h>
#include <aws/rds/model/BacktrackDBClusterRequest.h>
#include <aws/rds/model/CancelExportTaskRequest.h>
#include <aws/rds/model/CopyDBClusterParameterGroupRequest.h>
#include <aws/rds/model/CopyDBClusterSnapshotRequest.h>
#include <aws/rds/model/CopyDBParameterGroupRequest.h>
#include <aws/rds/model/CopyDBSnapshotRequest.h>
#include <aws/rds/model/CopyOptionGroupRequest.h>
#include <aws/rds/model/CreateCustomDBEngineVersionRequest.h>
#include <aws/rds/model/CreateDBClusterRequest.h>
#include <aws/rds/model/CreateDBClusterEndpointRequest.h>
#include <aws/rds/model/CreateDBClusterParameterGroupRequest.h>
#include <aws/rds/model/CreateDBClusterSnapshotRequest.h>
#include <aws/rds/model/CreateDBInstanceRequest.h>
#include <aws/rds/model/CreateDBInstanceReadReplicaRequest.h>
#include <aws/rds/model/CreateDBParameterGroupRequest.h>
#include <aws/rds/model/CreateDBProxyRequest.h>
#include <aws/rds/model/CreateDBProxyEndpointRequest.h>
#include <aws/rds/model/CreateDBSecurityGroupRequest.h>
#include <aws/rds/model/CreateDBSnapshotRequest.h>
#include <aws/rds/model/CreateDBSubnetGroupRequest.h>
#include <aws/rds/model/CreateEventSubscriptionRequest.h>
#include <aws/rds/model/CreateGlobalClusterRequest.h>
#include <aws/rds/model/CreateOptionGroupRequest.h>
#include <aws/rds/model/DeleteCustomDBEngineVersionRequest.h>
#include <aws/rds/model/DeleteDBClusterRequest.h>
#include <aws/rds/model/DeleteDBClusterEndpointRequest.h>
#include <aws/rds/model/DeleteDBClusterParameterGroupRequest.h>
#include <aws/rds/model/DeleteDBClusterSnapshotRequest.h>
#include <aws/rds/model/DeleteDBInstanceRequest.h>
#include <aws/rds/model/DeleteDBInstanceAutomatedBackupRequest.h>
#include <aws/rds/model/DeleteDBParameterGroupRequest.h>
#include <aws/rds/model/DeleteDBProxyRequest.h>
#include <aws/rds/model/DeleteDBProxyEndpointRequest.h>
#include <aws/rds/model/DeleteDBSecurityGroupRequest.h>
#include <aws/rds/model/DeleteDBSnapshotRequest.h>
#include <aws/rds/model/DeleteDBSubnetGroupRequest.h>
#include <aws/rds/model/DeleteEventSubscriptionRequest.h>
#include <aws/rds/model/DeleteGlobalClusterRequest.h>
#include <aws/rds/model/DeleteOptionGroupRequest.h>
#include <aws/rds/model/DeregisterDBProxyTargetsRequest.h>
#include <aws/rds/model/DescribeAccountAttributesRequest.h>
#include <aws/rds/model/DescribeCertificatesRequest.h>
#include <aws/rds/model/DescribeDBClusterBacktracksRequest.h>
#include <aws/rds/model/DescribeDBClusterEndpointsRequest.h>
#include <aws/rds/model/DescribeDBClusterParameterGroupsRequest.h>
#include <aws/rds/model/DescribeDBClusterParametersRequest.h>
#include <aws/rds/model/DescribeDBClusterSnapshotAttributesRequest.h>
#include <aws/rds/model/DescribeDBClusterSnapshotsRequest.h>
#include <aws/rds/model/DescribeDBClustersRequest.h>
#include <aws/rds/model/DescribeDBEngineVersionsRequest.h>
#include <aws/rds/model/DescribeDBInstanceAutomatedBackupsRequest.h>
#include <aws/rds/model/DescribeDBInstancesRequest.h>
#include <aws/rds/model/DescribeDBLogFilesRequest.h>
#include <aws/rds/model/DescribeDBParameterGroupsRequest.h>
#include <aws/rds/model/DescribeDBParametersRequest.h>
#include <aws/rds/model/DescribeDBProxiesRequest.h>
#include <aws/rds/model/DescribeDBProxyEndpointsRequest.h>
#include <aws/rds/model/DescribeDBProxyTargetGroupsRequest.h>
#include <aws/rds/model/DescribeDBProxyTargetsRequest.h>
#include <aws/rds/model/DescribeDBSecurityGroupsRequest.h>
#include <aws/rds/model/DescribeDBSnapshotAttributesRequest.h>
#include <aws/rds/model/DescribeDBSnapshotsRequest.h>
#include <aws/rds/model/DescribeDBSubnetGroupsRequest.h>
#include <aws/rds/model/DescribeEngineDefaultClusterParametersRequest.h>
#include <aws/rds/model/DescribeEngineDefaultParametersRequest.h>
#include <aws/rds/model/DescribeEventCategoriesRequest.h>
#include <aws/rds/model/DescribeEventSubscriptionsRequest.h>
#include <aws/rds/model/DescribeEventsRequest.h>
#include <aws/rds/model/DescribeExportTasksRequest.h>
#include <aws/rds/model/DescribeGlobalClustersRequest.h>
#include <aws/rds/model/DescribeOptionGroupOptionsRequest.h>
#include <aws/rds/model/DescribeOptionGroupsRequest.h>
#include <aws/rds/model/DescribeOrderableDBInstanceOptionsRequest.h>
#include <aws/rds/model/DescribePendingMaintenanceActionsRequest.h>
#include <aws/rds/model/DescribeReservedDBInstancesRequest.h>
#include <aws/rds/model/DescribeReservedDBInstancesOfferingsRequest.h>
#include <aws/rds/model/DescribeSourceRegionsRequest.h>
#include <aws/rds/model/DescribeValidDBInstanceModificationsRequest.h>
#include <aws/rds/model/DownloadDBLogFilePortionRequest.h>
#include <aws/rds/model/FailoverDBClusterRequest.h>
#include <aws/rds/model/FailoverGlobalClusterRequest.h>
#include <aws/rds/model/ListTagsForResourceRequest.h>
#include <aws/rds/model/ModifyActivityStreamRequest.h>
#include <aws/rds/model/ModifyCertificatesRequest.h>
#include <aws/rds/model/ModifyCurrentDBClusterCapacityRequest.h>
#include <aws/rds/model/ModifyCustomDBEngineVersionRequest.h>
#include <aws/rds/model/ModifyDBClusterRequest.h>
#include <aws/rds/model/ModifyDBClusterEndpointRequest.h>
#include <aws/rds/model/ModifyDBClusterParameterGroupRequest.h>
#include <aws/rds/model/ModifyDBClusterSnapshotAttributeRequest.h>
#include <aws/rds/model/ModifyDBInstanceRequest.h>
#include <aws/rds/model/ModifyDBParameterGroupRequest.h>
#include <aws/rds/model/ModifyDBProxyRequest.h>
#include <aws/rds/model/ModifyDBProxyEndpointRequest.h>
#include <aws/rds/model/ModifyDBProxyTargetGroupRequest.h>
#include <aws/rds/model/ModifyDBSnapshotRequest.h>
#include <aws/rds/model/ModifyDBSnapshotAttributeRequest.h>
#include <aws/rds/model/ModifyDBSubnetGroupRequest.h>
#include <aws/rds/model/ModifyEventSubscriptionRequest.h>
#include <aws/rds/model/ModifyGlobalClusterRequest.h>
#include <aws/rds/model/ModifyOptionGroupRequest.h>
#include <aws/rds/model/PromoteReadReplicaRequest.h>
#include <aws/rds/model/PromoteReadReplicaDBClusterRequest.h>
#include <aws/rds/model/PurchaseReservedDBInstancesOfferingRequest.h>
#include <aws/rds/model/RebootDBClusterRequest.h>
#include <aws/rds/model/RebootDBInstanceRequest.h>
#include <aws/rds/model/RegisterDBProxyTargetsRequest.h>
#include <aws/rds/model/RemoveFromGlobalClusterRequest.h>
#include <aws/rds/model/RemoveRoleFromDBClusterRequest.h>
#include <aws/rds/model/RemoveRoleFromDBInstanceRequest.h>
#include <aws/rds/model/RemoveSourceIdentifierFromSubscriptionRequest.h>
#include <aws/rds/model/RemoveTagsFromResourceRequest.h>
#include <aws/rds/model/ResetDBClusterParameterGroupRequest.h>
#include <aws/rds/model/ResetDBParameterGroupRequest.h>
#include <aws/rds/model/RestoreDBClusterFromS3Request.h>
#include <aws/rds/model/RestoreDBClusterFromSnapshotRequest.h>
#include <aws/rds/model/RestoreDBClusterToPointInTimeRequest.h>
#include <aws/rds/model/RestoreDBInstanceFromDBSnapshotRequest.h>
#include <aws/rds/model/RestoreDBInstanceFromS3Request.h>
#include <aws/rds/model/RestoreDBInstanceToPointInTimeRequest.h>
#include <aws/rds/model/RevokeDBSecurityGroupIngressRequest.h>
#include <aws/rds/model/StartActivityStreamRequest.h>
#include <aws/rds/model/StartDBClusterRequest.h>
#include <aws/rds/model/StartDBInstanceRequest.h>
#include <aws/rds/model/StartDBInstanceAutomatedBackupsReplicationRequest.h>
#include <aws/rds/model/StartExportTaskRequest.h>
#include <aws/rds/model/StopActivityStreamRequest.h>
#include <aws/rds/model/StopDBClusterRequest.h>
#include <aws/rds/model/StopDBInstanceRequest.h>
#include <aws/rds/model/StopDBInstanceAutomatedBackupsReplicationRequest.h>
#include <aws/rds/model/SwitchoverReadReplicaRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::RDS;
using namespace Aws::RDS::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Xml;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;


const char* RDSClient::SERVICE_NAME = "rds";
const char* RDSClient::ALLOCATION_TAG = "RDSClient";

RDSClient::RDSClient(const RDS::RDSClientConfiguration& clientConfiguration,
                     std::shared_ptr<RDSEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RDSClient::RDSClient(const AWSCredentials& credentials,
                     std::shared_ptr<RDSEndpointProviderBase> endpointProvider,
                     const RDS::RDSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RDSClient::RDSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     std::shared_ptr<RDSEndpointProviderBase> endpointProvider,
                     const RDS::RDSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  RDSClient::RDSClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<RDSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RDSClient::RDSClient(const AWSCredentials& credentials,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RDSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RDSClient::RDSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RDSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RDSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
RDSClient::~RDSClient()
{
}

std::shared_ptr<RDSEndpointProviderBase>& RDSClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void RDSClient::init(const RDS::RDSClientConfiguration& config)
{
  AWSClient::SetServiceClientName("RDS");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void RDSClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

Aws::String RDSClient::ConvertRequestToPresignedUrl(const AmazonSerializableWebServiceRequest& requestToConvert, const char* region) const
{
  if (!m_endpointProvider)
  {
    AWS_LOGSTREAM_ERROR(ALLOCATION_TAG, "Presigned URL generating failed. Endpoint provider is not initialized.");
    return "";
  }
  Aws::Endpoint::EndpointParameters endpointParameters;
  endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", Aws::String(region)));
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
  if (!endpointResolutionOutcome.IsSuccess())
  {
    AWS_LOGSTREAM_ERROR(ALLOCATION_TAG, "Endpoint resolution failed: " << endpointResolutionOutcome.GetError().GetMessage());
    return "";
  }
  Aws::StringStream ss;
  ss << "?" << requestToConvert.SerializePayload();
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());

  return GeneratePresignedUrl(endpointResolutionOutcome.GetResult().GetURI(), Aws::Http::HttpMethod::HTTP_GET, region, 3600);
}

AddRoleToDBClusterOutcome RDSClient::AddRoleToDBCluster(const AddRoleToDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddRoleToDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddRoleToDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddRoleToDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AddRoleToDBClusterOutcomeCallable RDSClient::AddRoleToDBClusterCallable(const AddRoleToDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::AddRoleToDBCluster, this, request, m_executor.get());
}

void RDSClient::AddRoleToDBClusterAsync(const AddRoleToDBClusterRequest& request, const AddRoleToDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::AddRoleToDBCluster, this, request, handler, context, m_executor.get());
}
AddRoleToDBInstanceOutcome RDSClient::AddRoleToDBInstance(const AddRoleToDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddRoleToDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddRoleToDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddRoleToDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AddRoleToDBInstanceOutcomeCallable RDSClient::AddRoleToDBInstanceCallable(const AddRoleToDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::AddRoleToDBInstance, this, request, m_executor.get());
}

void RDSClient::AddRoleToDBInstanceAsync(const AddRoleToDBInstanceRequest& request, const AddRoleToDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::AddRoleToDBInstance, this, request, handler, context, m_executor.get());
}
AddSourceIdentifierToSubscriptionOutcome RDSClient::AddSourceIdentifierToSubscription(const AddSourceIdentifierToSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddSourceIdentifierToSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddSourceIdentifierToSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddSourceIdentifierToSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AddSourceIdentifierToSubscriptionOutcomeCallable RDSClient::AddSourceIdentifierToSubscriptionCallable(const AddSourceIdentifierToSubscriptionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::AddSourceIdentifierToSubscription, this, request, m_executor.get());
}

void RDSClient::AddSourceIdentifierToSubscriptionAsync(const AddSourceIdentifierToSubscriptionRequest& request, const AddSourceIdentifierToSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::AddSourceIdentifierToSubscription, this, request, handler, context, m_executor.get());
}
AddTagsToResourceOutcome RDSClient::AddTagsToResource(const AddTagsToResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddTagsToResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddTagsToResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddTagsToResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AddTagsToResourceOutcomeCallable RDSClient::AddTagsToResourceCallable(const AddTagsToResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::AddTagsToResource, this, request, m_executor.get());
}

void RDSClient::AddTagsToResourceAsync(const AddTagsToResourceRequest& request, const AddTagsToResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::AddTagsToResource, this, request, handler, context, m_executor.get());
}
ApplyPendingMaintenanceActionOutcome RDSClient::ApplyPendingMaintenanceAction(const ApplyPendingMaintenanceActionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ApplyPendingMaintenanceAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ApplyPendingMaintenanceAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ApplyPendingMaintenanceActionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ApplyPendingMaintenanceActionOutcomeCallable RDSClient::ApplyPendingMaintenanceActionCallable(const ApplyPendingMaintenanceActionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ApplyPendingMaintenanceAction, this, request, m_executor.get());
}

void RDSClient::ApplyPendingMaintenanceActionAsync(const ApplyPendingMaintenanceActionRequest& request, const ApplyPendingMaintenanceActionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ApplyPendingMaintenanceAction, this, request, handler, context, m_executor.get());
}
AuthorizeDBSecurityGroupIngressOutcome RDSClient::AuthorizeDBSecurityGroupIngress(const AuthorizeDBSecurityGroupIngressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeDBSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeDBSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeDBSecurityGroupIngressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AuthorizeDBSecurityGroupIngressOutcomeCallable RDSClient::AuthorizeDBSecurityGroupIngressCallable(const AuthorizeDBSecurityGroupIngressRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::AuthorizeDBSecurityGroupIngress, this, request, m_executor.get());
}

void RDSClient::AuthorizeDBSecurityGroupIngressAsync(const AuthorizeDBSecurityGroupIngressRequest& request, const AuthorizeDBSecurityGroupIngressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::AuthorizeDBSecurityGroupIngress, this, request, handler, context, m_executor.get());
}
BacktrackDBClusterOutcome RDSClient::BacktrackDBCluster(const BacktrackDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BacktrackDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BacktrackDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return BacktrackDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

BacktrackDBClusterOutcomeCallable RDSClient::BacktrackDBClusterCallable(const BacktrackDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::BacktrackDBCluster, this, request, m_executor.get());
}

void RDSClient::BacktrackDBClusterAsync(const BacktrackDBClusterRequest& request, const BacktrackDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::BacktrackDBCluster, this, request, handler, context, m_executor.get());
}
CancelExportTaskOutcome RDSClient::CancelExportTask(const CancelExportTaskRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelExportTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelExportTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CancelExportTaskOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CancelExportTaskOutcomeCallable RDSClient::CancelExportTaskCallable(const CancelExportTaskRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CancelExportTask, this, request, m_executor.get());
}

void RDSClient::CancelExportTaskAsync(const CancelExportTaskRequest& request, const CancelExportTaskResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CancelExportTask, this, request, handler, context, m_executor.get());
}
CopyDBClusterParameterGroupOutcome RDSClient::CopyDBClusterParameterGroup(const CopyDBClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyDBClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyDBClusterParameterGroupOutcomeCallable RDSClient::CopyDBClusterParameterGroupCallable(const CopyDBClusterParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CopyDBClusterParameterGroup, this, request, m_executor.get());
}

void RDSClient::CopyDBClusterParameterGroupAsync(const CopyDBClusterParameterGroupRequest& request, const CopyDBClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CopyDBClusterParameterGroup, this, request, handler, context, m_executor.get());
}
CopyDBClusterSnapshotOutcome RDSClient::CopyDBClusterSnapshot(const CopyDBClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  CopyDBClusterSnapshotRequest newRequest = request;
  if (request.SourceRegionHasBeenSet() && !request.PreSignedUrlHasBeenSet())
  {
    Aws::Endpoint::EndpointParameters endpointParameters;
    endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", request.GetSourceRegion()));
    ResolveEndpointOutcome presignedEndpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
    AWS_OPERATION_CHECK_SUCCESS(presignedEndpointResolutionOutcome, CopyDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, presignedEndpointResolutionOutcome.GetError().GetMessage());
    newRequest.SetPreSignedUrl(GeneratePresignedUrl(request, presignedEndpointResolutionOutcome.GetResult().GetURI(),
                                                    Aws::Http::HttpMethod::HTTP_GET, request.GetSourceRegion().c_str(),
                                                    {{ "DestinationRegion", m_region }}, 3600));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyDBClusterSnapshotOutcome(MakeRequest(newRequest, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyDBClusterSnapshotOutcomeCallable RDSClient::CopyDBClusterSnapshotCallable(const CopyDBClusterSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CopyDBClusterSnapshot, this, request, m_executor.get());
}

void RDSClient::CopyDBClusterSnapshotAsync(const CopyDBClusterSnapshotRequest& request, const CopyDBClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CopyDBClusterSnapshot, this, request, handler, context, m_executor.get());
}
CopyDBParameterGroupOutcome RDSClient::CopyDBParameterGroup(const CopyDBParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyDBParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyDBParameterGroupOutcomeCallable RDSClient::CopyDBParameterGroupCallable(const CopyDBParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CopyDBParameterGroup, this, request, m_executor.get());
}

void RDSClient::CopyDBParameterGroupAsync(const CopyDBParameterGroupRequest& request, const CopyDBParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CopyDBParameterGroup, this, request, handler, context, m_executor.get());
}
CopyDBSnapshotOutcome RDSClient::CopyDBSnapshot(const CopyDBSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  CopyDBSnapshotRequest newRequest = request;
  if (request.SourceRegionHasBeenSet() && !request.PreSignedUrlHasBeenSet())
  {
    Aws::Endpoint::EndpointParameters endpointParameters;
    endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", request.GetSourceRegion()));
    ResolveEndpointOutcome presignedEndpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
    AWS_OPERATION_CHECK_SUCCESS(presignedEndpointResolutionOutcome, CopyDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, presignedEndpointResolutionOutcome.GetError().GetMessage());
    newRequest.SetPreSignedUrl(GeneratePresignedUrl(request, presignedEndpointResolutionOutcome.GetResult().GetURI(),
                                                    Aws::Http::HttpMethod::HTTP_GET, request.GetSourceRegion().c_str(),
                                                    {{ "DestinationRegion", m_region }}, 3600));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyDBSnapshotOutcome(MakeRequest(newRequest, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyDBSnapshotOutcomeCallable RDSClient::CopyDBSnapshotCallable(const CopyDBSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CopyDBSnapshot, this, request, m_executor.get());
}

void RDSClient::CopyDBSnapshotAsync(const CopyDBSnapshotRequest& request, const CopyDBSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CopyDBSnapshot, this, request, handler, context, m_executor.get());
}
CopyOptionGroupOutcome RDSClient::CopyOptionGroup(const CopyOptionGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyOptionGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyOptionGroupOutcomeCallable RDSClient::CopyOptionGroupCallable(const CopyOptionGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CopyOptionGroup, this, request, m_executor.get());
}

void RDSClient::CopyOptionGroupAsync(const CopyOptionGroupRequest& request, const CopyOptionGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CopyOptionGroup, this, request, handler, context, m_executor.get());
}
CreateCustomDBEngineVersionOutcome RDSClient::CreateCustomDBEngineVersion(const CreateCustomDBEngineVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateCustomDBEngineVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateCustomDBEngineVersionOutcomeCallable RDSClient::CreateCustomDBEngineVersionCallable(const CreateCustomDBEngineVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateCustomDBEngineVersion, this, request, m_executor.get());
}

void RDSClient::CreateCustomDBEngineVersionAsync(const CreateCustomDBEngineVersionRequest& request, const CreateCustomDBEngineVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateCustomDBEngineVersion, this, request, handler, context, m_executor.get());
}
CreateDBClusterOutcome RDSClient::CreateDBCluster(const CreateDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  CreateDBClusterRequest newRequest = request;
  if (request.SourceRegionHasBeenSet() && !request.PreSignedUrlHasBeenSet())
  {
    Aws::Endpoint::EndpointParameters endpointParameters;
    endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", request.GetSourceRegion()));
    ResolveEndpointOutcome presignedEndpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
    AWS_OPERATION_CHECK_SUCCESS(presignedEndpointResolutionOutcome, CreateDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, presignedEndpointResolutionOutcome.GetError().GetMessage());
    newRequest.SetPreSignedUrl(GeneratePresignedUrl(request, presignedEndpointResolutionOutcome.GetResult().GetURI(),
                                                    Aws::Http::HttpMethod::HTTP_GET, request.GetSourceRegion().c_str(),
                                                    {{ "DestinationRegion", m_region }}, 3600));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBClusterOutcome(MakeRequest(newRequest, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBClusterOutcomeCallable RDSClient::CreateDBClusterCallable(const CreateDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBCluster, this, request, m_executor.get());
}

void RDSClient::CreateDBClusterAsync(const CreateDBClusterRequest& request, const CreateDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBCluster, this, request, handler, context, m_executor.get());
}
CreateDBClusterEndpointOutcome RDSClient::CreateDBClusterEndpoint(const CreateDBClusterEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBClusterEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBClusterEndpointOutcomeCallable RDSClient::CreateDBClusterEndpointCallable(const CreateDBClusterEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBClusterEndpoint, this, request, m_executor.get());
}

void RDSClient::CreateDBClusterEndpointAsync(const CreateDBClusterEndpointRequest& request, const CreateDBClusterEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBClusterEndpoint, this, request, handler, context, m_executor.get());
}
CreateDBClusterParameterGroupOutcome RDSClient::CreateDBClusterParameterGroup(const CreateDBClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBClusterParameterGroupOutcomeCallable RDSClient::CreateDBClusterParameterGroupCallable(const CreateDBClusterParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBClusterParameterGroup, this, request, m_executor.get());
}

void RDSClient::CreateDBClusterParameterGroupAsync(const CreateDBClusterParameterGroupRequest& request, const CreateDBClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBClusterParameterGroup, this, request, handler, context, m_executor.get());
}
CreateDBClusterSnapshotOutcome RDSClient::CreateDBClusterSnapshot(const CreateDBClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBClusterSnapshotOutcomeCallable RDSClient::CreateDBClusterSnapshotCallable(const CreateDBClusterSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBClusterSnapshot, this, request, m_executor.get());
}

void RDSClient::CreateDBClusterSnapshotAsync(const CreateDBClusterSnapshotRequest& request, const CreateDBClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBClusterSnapshot, this, request, handler, context, m_executor.get());
}
CreateDBInstanceOutcome RDSClient::CreateDBInstance(const CreateDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBInstanceOutcomeCallable RDSClient::CreateDBInstanceCallable(const CreateDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBInstance, this, request, m_executor.get());
}

void RDSClient::CreateDBInstanceAsync(const CreateDBInstanceRequest& request, const CreateDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBInstance, this, request, handler, context, m_executor.get());
}
CreateDBInstanceReadReplicaOutcome RDSClient::CreateDBInstanceReadReplica(const CreateDBInstanceReadReplicaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBInstanceReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  CreateDBInstanceReadReplicaRequest newRequest = request;
  if (request.SourceRegionHasBeenSet() && !request.PreSignedUrlHasBeenSet())
  {
    Aws::Endpoint::EndpointParameters endpointParameters;
    endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", request.GetSourceRegion()));
    ResolveEndpointOutcome presignedEndpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
    AWS_OPERATION_CHECK_SUCCESS(presignedEndpointResolutionOutcome, CreateDBInstanceReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, presignedEndpointResolutionOutcome.GetError().GetMessage());
    newRequest.SetPreSignedUrl(GeneratePresignedUrl(request, presignedEndpointResolutionOutcome.GetResult().GetURI(),
                                                    Aws::Http::HttpMethod::HTTP_GET, request.GetSourceRegion().c_str(),
                                                    {{ "DestinationRegion", m_region }}, 3600));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBInstanceReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBInstanceReadReplicaOutcome(MakeRequest(newRequest, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBInstanceReadReplicaOutcomeCallable RDSClient::CreateDBInstanceReadReplicaCallable(const CreateDBInstanceReadReplicaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBInstanceReadReplica, this, request, m_executor.get());
}

void RDSClient::CreateDBInstanceReadReplicaAsync(const CreateDBInstanceReadReplicaRequest& request, const CreateDBInstanceReadReplicaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBInstanceReadReplica, this, request, handler, context, m_executor.get());
}
CreateDBParameterGroupOutcome RDSClient::CreateDBParameterGroup(const CreateDBParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBParameterGroupOutcomeCallable RDSClient::CreateDBParameterGroupCallable(const CreateDBParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBParameterGroup, this, request, m_executor.get());
}

void RDSClient::CreateDBParameterGroupAsync(const CreateDBParameterGroupRequest& request, const CreateDBParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBParameterGroup, this, request, handler, context, m_executor.get());
}
CreateDBProxyOutcome RDSClient::CreateDBProxy(const CreateDBProxyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBProxyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBProxyOutcomeCallable RDSClient::CreateDBProxyCallable(const CreateDBProxyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBProxy, this, request, m_executor.get());
}

void RDSClient::CreateDBProxyAsync(const CreateDBProxyRequest& request, const CreateDBProxyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBProxy, this, request, handler, context, m_executor.get());
}
CreateDBProxyEndpointOutcome RDSClient::CreateDBProxyEndpoint(const CreateDBProxyEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBProxyEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBProxyEndpointOutcomeCallable RDSClient::CreateDBProxyEndpointCallable(const CreateDBProxyEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBProxyEndpoint, this, request, m_executor.get());
}

void RDSClient::CreateDBProxyEndpointAsync(const CreateDBProxyEndpointRequest& request, const CreateDBProxyEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBProxyEndpoint, this, request, handler, context, m_executor.get());
}
CreateDBSecurityGroupOutcome RDSClient::CreateDBSecurityGroup(const CreateDBSecurityGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBSecurityGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBSecurityGroupOutcomeCallable RDSClient::CreateDBSecurityGroupCallable(const CreateDBSecurityGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBSecurityGroup, this, request, m_executor.get());
}

void RDSClient::CreateDBSecurityGroupAsync(const CreateDBSecurityGroupRequest& request, const CreateDBSecurityGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBSecurityGroup, this, request, handler, context, m_executor.get());
}
CreateDBSnapshotOutcome RDSClient::CreateDBSnapshot(const CreateDBSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBSnapshotOutcomeCallable RDSClient::CreateDBSnapshotCallable(const CreateDBSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBSnapshot, this, request, m_executor.get());
}

void RDSClient::CreateDBSnapshotAsync(const CreateDBSnapshotRequest& request, const CreateDBSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBSnapshot, this, request, handler, context, m_executor.get());
}
CreateDBSubnetGroupOutcome RDSClient::CreateDBSubnetGroup(const CreateDBSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateDBSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateDBSubnetGroupOutcomeCallable RDSClient::CreateDBSubnetGroupCallable(const CreateDBSubnetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateDBSubnetGroup, this, request, m_executor.get());
}

void RDSClient::CreateDBSubnetGroupAsync(const CreateDBSubnetGroupRequest& request, const CreateDBSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateDBSubnetGroup, this, request, handler, context, m_executor.get());
}
CreateEventSubscriptionOutcome RDSClient::CreateEventSubscription(const CreateEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateEventSubscriptionOutcomeCallable RDSClient::CreateEventSubscriptionCallable(const CreateEventSubscriptionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateEventSubscription, this, request, m_executor.get());
}

void RDSClient::CreateEventSubscriptionAsync(const CreateEventSubscriptionRequest& request, const CreateEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateEventSubscription, this, request, handler, context, m_executor.get());
}
CreateGlobalClusterOutcome RDSClient::CreateGlobalCluster(const CreateGlobalClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateGlobalClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateGlobalClusterOutcomeCallable RDSClient::CreateGlobalClusterCallable(const CreateGlobalClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateGlobalCluster, this, request, m_executor.get());
}

void RDSClient::CreateGlobalClusterAsync(const CreateGlobalClusterRequest& request, const CreateGlobalClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateGlobalCluster, this, request, handler, context, m_executor.get());
}
CreateOptionGroupOutcome RDSClient::CreateOptionGroup(const CreateOptionGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateOptionGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateOptionGroupOutcomeCallable RDSClient::CreateOptionGroupCallable(const CreateOptionGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::CreateOptionGroup, this, request, m_executor.get());
}

void RDSClient::CreateOptionGroupAsync(const CreateOptionGroupRequest& request, const CreateOptionGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::CreateOptionGroup, this, request, handler, context, m_executor.get());
}
DeleteCustomDBEngineVersionOutcome RDSClient::DeleteCustomDBEngineVersion(const DeleteCustomDBEngineVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteCustomDBEngineVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteCustomDBEngineVersionOutcomeCallable RDSClient::DeleteCustomDBEngineVersionCallable(const DeleteCustomDBEngineVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteCustomDBEngineVersion, this, request, m_executor.get());
}

void RDSClient::DeleteCustomDBEngineVersionAsync(const DeleteCustomDBEngineVersionRequest& request, const DeleteCustomDBEngineVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteCustomDBEngineVersion, this, request, handler, context, m_executor.get());
}
DeleteDBClusterOutcome RDSClient::DeleteDBCluster(const DeleteDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBClusterOutcomeCallable RDSClient::DeleteDBClusterCallable(const DeleteDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBCluster, this, request, m_executor.get());
}

void RDSClient::DeleteDBClusterAsync(const DeleteDBClusterRequest& request, const DeleteDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBCluster, this, request, handler, context, m_executor.get());
}
DeleteDBClusterEndpointOutcome RDSClient::DeleteDBClusterEndpoint(const DeleteDBClusterEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBClusterEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBClusterEndpointOutcomeCallable RDSClient::DeleteDBClusterEndpointCallable(const DeleteDBClusterEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBClusterEndpoint, this, request, m_executor.get());
}

void RDSClient::DeleteDBClusterEndpointAsync(const DeleteDBClusterEndpointRequest& request, const DeleteDBClusterEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBClusterEndpoint, this, request, handler, context, m_executor.get());
}
DeleteDBClusterParameterGroupOutcome RDSClient::DeleteDBClusterParameterGroup(const DeleteDBClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBClusterParameterGroupOutcomeCallable RDSClient::DeleteDBClusterParameterGroupCallable(const DeleteDBClusterParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBClusterParameterGroup, this, request, m_executor.get());
}

void RDSClient::DeleteDBClusterParameterGroupAsync(const DeleteDBClusterParameterGroupRequest& request, const DeleteDBClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBClusterParameterGroup, this, request, handler, context, m_executor.get());
}
DeleteDBClusterSnapshotOutcome RDSClient::DeleteDBClusterSnapshot(const DeleteDBClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBClusterSnapshotOutcomeCallable RDSClient::DeleteDBClusterSnapshotCallable(const DeleteDBClusterSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBClusterSnapshot, this, request, m_executor.get());
}

void RDSClient::DeleteDBClusterSnapshotAsync(const DeleteDBClusterSnapshotRequest& request, const DeleteDBClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBClusterSnapshot, this, request, handler, context, m_executor.get());
}
DeleteDBInstanceOutcome RDSClient::DeleteDBInstance(const DeleteDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBInstanceOutcomeCallable RDSClient::DeleteDBInstanceCallable(const DeleteDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBInstance, this, request, m_executor.get());
}

void RDSClient::DeleteDBInstanceAsync(const DeleteDBInstanceRequest& request, const DeleteDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBInstance, this, request, handler, context, m_executor.get());
}
DeleteDBInstanceAutomatedBackupOutcome RDSClient::DeleteDBInstanceAutomatedBackup(const DeleteDBInstanceAutomatedBackupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBInstanceAutomatedBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBInstanceAutomatedBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBInstanceAutomatedBackupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBInstanceAutomatedBackupOutcomeCallable RDSClient::DeleteDBInstanceAutomatedBackupCallable(const DeleteDBInstanceAutomatedBackupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBInstanceAutomatedBackup, this, request, m_executor.get());
}

void RDSClient::DeleteDBInstanceAutomatedBackupAsync(const DeleteDBInstanceAutomatedBackupRequest& request, const DeleteDBInstanceAutomatedBackupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBInstanceAutomatedBackup, this, request, handler, context, m_executor.get());
}
DeleteDBParameterGroupOutcome RDSClient::DeleteDBParameterGroup(const DeleteDBParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBParameterGroupOutcomeCallable RDSClient::DeleteDBParameterGroupCallable(const DeleteDBParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBParameterGroup, this, request, m_executor.get());
}

void RDSClient::DeleteDBParameterGroupAsync(const DeleteDBParameterGroupRequest& request, const DeleteDBParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBParameterGroup, this, request, handler, context, m_executor.get());
}
DeleteDBProxyOutcome RDSClient::DeleteDBProxy(const DeleteDBProxyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBProxyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBProxyOutcomeCallable RDSClient::DeleteDBProxyCallable(const DeleteDBProxyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBProxy, this, request, m_executor.get());
}

void RDSClient::DeleteDBProxyAsync(const DeleteDBProxyRequest& request, const DeleteDBProxyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBProxy, this, request, handler, context, m_executor.get());
}
DeleteDBProxyEndpointOutcome RDSClient::DeleteDBProxyEndpoint(const DeleteDBProxyEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBProxyEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBProxyEndpointOutcomeCallable RDSClient::DeleteDBProxyEndpointCallable(const DeleteDBProxyEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBProxyEndpoint, this, request, m_executor.get());
}

void RDSClient::DeleteDBProxyEndpointAsync(const DeleteDBProxyEndpointRequest& request, const DeleteDBProxyEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBProxyEndpoint, this, request, handler, context, m_executor.get());
}
DeleteDBSecurityGroupOutcome RDSClient::DeleteDBSecurityGroup(const DeleteDBSecurityGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBSecurityGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBSecurityGroupOutcomeCallable RDSClient::DeleteDBSecurityGroupCallable(const DeleteDBSecurityGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBSecurityGroup, this, request, m_executor.get());
}

void RDSClient::DeleteDBSecurityGroupAsync(const DeleteDBSecurityGroupRequest& request, const DeleteDBSecurityGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBSecurityGroup, this, request, handler, context, m_executor.get());
}
DeleteDBSnapshotOutcome RDSClient::DeleteDBSnapshot(const DeleteDBSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBSnapshotOutcomeCallable RDSClient::DeleteDBSnapshotCallable(const DeleteDBSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBSnapshot, this, request, m_executor.get());
}

void RDSClient::DeleteDBSnapshotAsync(const DeleteDBSnapshotRequest& request, const DeleteDBSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBSnapshot, this, request, handler, context, m_executor.get());
}
DeleteDBSubnetGroupOutcome RDSClient::DeleteDBSubnetGroup(const DeleteDBSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteDBSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteDBSubnetGroupOutcomeCallable RDSClient::DeleteDBSubnetGroupCallable(const DeleteDBSubnetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteDBSubnetGroup, this, request, m_executor.get());
}

void RDSClient::DeleteDBSubnetGroupAsync(const DeleteDBSubnetGroupRequest& request, const DeleteDBSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteDBSubnetGroup, this, request, handler, context, m_executor.get());
}
DeleteEventSubscriptionOutcome RDSClient::DeleteEventSubscription(const DeleteEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteEventSubscriptionOutcomeCallable RDSClient::DeleteEventSubscriptionCallable(const DeleteEventSubscriptionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteEventSubscription, this, request, m_executor.get());
}

void RDSClient::DeleteEventSubscriptionAsync(const DeleteEventSubscriptionRequest& request, const DeleteEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteEventSubscription, this, request, handler, context, m_executor.get());
}
DeleteGlobalClusterOutcome RDSClient::DeleteGlobalCluster(const DeleteGlobalClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteGlobalClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteGlobalClusterOutcomeCallable RDSClient::DeleteGlobalClusterCallable(const DeleteGlobalClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteGlobalCluster, this, request, m_executor.get());
}

void RDSClient::DeleteGlobalClusterAsync(const DeleteGlobalClusterRequest& request, const DeleteGlobalClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteGlobalCluster, this, request, handler, context, m_executor.get());
}
DeleteOptionGroupOutcome RDSClient::DeleteOptionGroup(const DeleteOptionGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteOptionGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteOptionGroupOutcomeCallable RDSClient::DeleteOptionGroupCallable(const DeleteOptionGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeleteOptionGroup, this, request, m_executor.get());
}

void RDSClient::DeleteOptionGroupAsync(const DeleteOptionGroupRequest& request, const DeleteOptionGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeleteOptionGroup, this, request, handler, context, m_executor.get());
}
DeregisterDBProxyTargetsOutcome RDSClient::DeregisterDBProxyTargets(const DeregisterDBProxyTargetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeregisterDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeregisterDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeregisterDBProxyTargetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeregisterDBProxyTargetsOutcomeCallable RDSClient::DeregisterDBProxyTargetsCallable(const DeregisterDBProxyTargetsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DeregisterDBProxyTargets, this, request, m_executor.get());
}

void RDSClient::DeregisterDBProxyTargetsAsync(const DeregisterDBProxyTargetsRequest& request, const DeregisterDBProxyTargetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DeregisterDBProxyTargets, this, request, handler, context, m_executor.get());
}
DescribeAccountAttributesOutcome RDSClient::DescribeAccountAttributes(const DescribeAccountAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeAccountAttributesOutcomeCallable RDSClient::DescribeAccountAttributesCallable(const DescribeAccountAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeAccountAttributes, this, request, m_executor.get());
}

void RDSClient::DescribeAccountAttributesAsync(const DescribeAccountAttributesRequest& request, const DescribeAccountAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeAccountAttributes, this, request, handler, context, m_executor.get());
}
DescribeCertificatesOutcome RDSClient::DescribeCertificates(const DescribeCertificatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeCertificatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeCertificatesOutcomeCallable RDSClient::DescribeCertificatesCallable(const DescribeCertificatesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeCertificates, this, request, m_executor.get());
}

void RDSClient::DescribeCertificatesAsync(const DescribeCertificatesRequest& request, const DescribeCertificatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeCertificates, this, request, handler, context, m_executor.get());
}
DescribeDBClusterBacktracksOutcome RDSClient::DescribeDBClusterBacktracks(const DescribeDBClusterBacktracksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterBacktracks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterBacktracks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterBacktracksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterBacktracksOutcomeCallable RDSClient::DescribeDBClusterBacktracksCallable(const DescribeDBClusterBacktracksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterBacktracks, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterBacktracksAsync(const DescribeDBClusterBacktracksRequest& request, const DescribeDBClusterBacktracksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterBacktracks, this, request, handler, context, m_executor.get());
}
DescribeDBClusterEndpointsOutcome RDSClient::DescribeDBClusterEndpoints(const DescribeDBClusterEndpointsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterEndpoints, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterEndpoints, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterEndpointsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterEndpointsOutcomeCallable RDSClient::DescribeDBClusterEndpointsCallable(const DescribeDBClusterEndpointsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterEndpoints, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterEndpointsAsync(const DescribeDBClusterEndpointsRequest& request, const DescribeDBClusterEndpointsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterEndpoints, this, request, handler, context, m_executor.get());
}
DescribeDBClusterParameterGroupsOutcome RDSClient::DescribeDBClusterParameterGroups(const DescribeDBClusterParameterGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterParameterGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterParameterGroupsOutcomeCallable RDSClient::DescribeDBClusterParameterGroupsCallable(const DescribeDBClusterParameterGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterParameterGroups, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterParameterGroupsAsync(const DescribeDBClusterParameterGroupsRequest& request, const DescribeDBClusterParameterGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterParameterGroups, this, request, handler, context, m_executor.get());
}
DescribeDBClusterParametersOutcome RDSClient::DescribeDBClusterParameters(const DescribeDBClusterParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterParametersOutcomeCallable RDSClient::DescribeDBClusterParametersCallable(const DescribeDBClusterParametersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterParameters, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterParametersAsync(const DescribeDBClusterParametersRequest& request, const DescribeDBClusterParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterParameters, this, request, handler, context, m_executor.get());
}
DescribeDBClusterSnapshotAttributesOutcome RDSClient::DescribeDBClusterSnapshotAttributes(const DescribeDBClusterSnapshotAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterSnapshotAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterSnapshotAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterSnapshotAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterSnapshotAttributesOutcomeCallable RDSClient::DescribeDBClusterSnapshotAttributesCallable(const DescribeDBClusterSnapshotAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterSnapshotAttributes, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterSnapshotAttributesAsync(const DescribeDBClusterSnapshotAttributesRequest& request, const DescribeDBClusterSnapshotAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterSnapshotAttributes, this, request, handler, context, m_executor.get());
}
DescribeDBClusterSnapshotsOutcome RDSClient::DescribeDBClusterSnapshots(const DescribeDBClusterSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClusterSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClusterSnapshotsOutcomeCallable RDSClient::DescribeDBClusterSnapshotsCallable(const DescribeDBClusterSnapshotsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusterSnapshots, this, request, m_executor.get());
}

void RDSClient::DescribeDBClusterSnapshotsAsync(const DescribeDBClusterSnapshotsRequest& request, const DescribeDBClusterSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusterSnapshots, this, request, handler, context, m_executor.get());
}
DescribeDBClustersOutcome RDSClient::DescribeDBClusters(const DescribeDBClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBClustersOutcomeCallable RDSClient::DescribeDBClustersCallable(const DescribeDBClustersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBClusters, this, request, m_executor.get());
}

void RDSClient::DescribeDBClustersAsync(const DescribeDBClustersRequest& request, const DescribeDBClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBClusters, this, request, handler, context, m_executor.get());
}
DescribeDBEngineVersionsOutcome RDSClient::DescribeDBEngineVersions(const DescribeDBEngineVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBEngineVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBEngineVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBEngineVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBEngineVersionsOutcomeCallable RDSClient::DescribeDBEngineVersionsCallable(const DescribeDBEngineVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBEngineVersions, this, request, m_executor.get());
}

void RDSClient::DescribeDBEngineVersionsAsync(const DescribeDBEngineVersionsRequest& request, const DescribeDBEngineVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBEngineVersions, this, request, handler, context, m_executor.get());
}
DescribeDBInstanceAutomatedBackupsOutcome RDSClient::DescribeDBInstanceAutomatedBackups(const DescribeDBInstanceAutomatedBackupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBInstanceAutomatedBackups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBInstanceAutomatedBackups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBInstanceAutomatedBackupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBInstanceAutomatedBackupsOutcomeCallable RDSClient::DescribeDBInstanceAutomatedBackupsCallable(const DescribeDBInstanceAutomatedBackupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBInstanceAutomatedBackups, this, request, m_executor.get());
}

void RDSClient::DescribeDBInstanceAutomatedBackupsAsync(const DescribeDBInstanceAutomatedBackupsRequest& request, const DescribeDBInstanceAutomatedBackupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBInstanceAutomatedBackups, this, request, handler, context, m_executor.get());
}
DescribeDBInstancesOutcome RDSClient::DescribeDBInstances(const DescribeDBInstancesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBInstancesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBInstancesOutcomeCallable RDSClient::DescribeDBInstancesCallable(const DescribeDBInstancesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBInstances, this, request, m_executor.get());
}

void RDSClient::DescribeDBInstancesAsync(const DescribeDBInstancesRequest& request, const DescribeDBInstancesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBInstances, this, request, handler, context, m_executor.get());
}
DescribeDBLogFilesOutcome RDSClient::DescribeDBLogFiles(const DescribeDBLogFilesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBLogFiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBLogFiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBLogFilesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBLogFilesOutcomeCallable RDSClient::DescribeDBLogFilesCallable(const DescribeDBLogFilesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBLogFiles, this, request, m_executor.get());
}

void RDSClient::DescribeDBLogFilesAsync(const DescribeDBLogFilesRequest& request, const DescribeDBLogFilesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBLogFiles, this, request, handler, context, m_executor.get());
}
DescribeDBParameterGroupsOutcome RDSClient::DescribeDBParameterGroups(const DescribeDBParameterGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBParameterGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBParameterGroupsOutcomeCallable RDSClient::DescribeDBParameterGroupsCallable(const DescribeDBParameterGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBParameterGroups, this, request, m_executor.get());
}

void RDSClient::DescribeDBParameterGroupsAsync(const DescribeDBParameterGroupsRequest& request, const DescribeDBParameterGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBParameterGroups, this, request, handler, context, m_executor.get());
}
DescribeDBParametersOutcome RDSClient::DescribeDBParameters(const DescribeDBParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBParametersOutcomeCallable RDSClient::DescribeDBParametersCallable(const DescribeDBParametersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBParameters, this, request, m_executor.get());
}

void RDSClient::DescribeDBParametersAsync(const DescribeDBParametersRequest& request, const DescribeDBParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBParameters, this, request, handler, context, m_executor.get());
}
DescribeDBProxiesOutcome RDSClient::DescribeDBProxies(const DescribeDBProxiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBProxies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBProxies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBProxiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBProxiesOutcomeCallable RDSClient::DescribeDBProxiesCallable(const DescribeDBProxiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBProxies, this, request, m_executor.get());
}

void RDSClient::DescribeDBProxiesAsync(const DescribeDBProxiesRequest& request, const DescribeDBProxiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBProxies, this, request, handler, context, m_executor.get());
}
DescribeDBProxyEndpointsOutcome RDSClient::DescribeDBProxyEndpoints(const DescribeDBProxyEndpointsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBProxyEndpoints, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBProxyEndpoints, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBProxyEndpointsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBProxyEndpointsOutcomeCallable RDSClient::DescribeDBProxyEndpointsCallable(const DescribeDBProxyEndpointsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBProxyEndpoints, this, request, m_executor.get());
}

void RDSClient::DescribeDBProxyEndpointsAsync(const DescribeDBProxyEndpointsRequest& request, const DescribeDBProxyEndpointsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBProxyEndpoints, this, request, handler, context, m_executor.get());
}
DescribeDBProxyTargetGroupsOutcome RDSClient::DescribeDBProxyTargetGroups(const DescribeDBProxyTargetGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBProxyTargetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBProxyTargetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBProxyTargetGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBProxyTargetGroupsOutcomeCallable RDSClient::DescribeDBProxyTargetGroupsCallable(const DescribeDBProxyTargetGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBProxyTargetGroups, this, request, m_executor.get());
}

void RDSClient::DescribeDBProxyTargetGroupsAsync(const DescribeDBProxyTargetGroupsRequest& request, const DescribeDBProxyTargetGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBProxyTargetGroups, this, request, handler, context, m_executor.get());
}
DescribeDBProxyTargetsOutcome RDSClient::DescribeDBProxyTargets(const DescribeDBProxyTargetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBProxyTargetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBProxyTargetsOutcomeCallable RDSClient::DescribeDBProxyTargetsCallable(const DescribeDBProxyTargetsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBProxyTargets, this, request, m_executor.get());
}

void RDSClient::DescribeDBProxyTargetsAsync(const DescribeDBProxyTargetsRequest& request, const DescribeDBProxyTargetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBProxyTargets, this, request, handler, context, m_executor.get());
}
DescribeDBSecurityGroupsOutcome RDSClient::DescribeDBSecurityGroups(const DescribeDBSecurityGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBSecurityGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBSecurityGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBSecurityGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBSecurityGroupsOutcomeCallable RDSClient::DescribeDBSecurityGroupsCallable(const DescribeDBSecurityGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBSecurityGroups, this, request, m_executor.get());
}

void RDSClient::DescribeDBSecurityGroupsAsync(const DescribeDBSecurityGroupsRequest& request, const DescribeDBSecurityGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBSecurityGroups, this, request, handler, context, m_executor.get());
}
DescribeDBSnapshotAttributesOutcome RDSClient::DescribeDBSnapshotAttributes(const DescribeDBSnapshotAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBSnapshotAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBSnapshotAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBSnapshotAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBSnapshotAttributesOutcomeCallable RDSClient::DescribeDBSnapshotAttributesCallable(const DescribeDBSnapshotAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBSnapshotAttributes, this, request, m_executor.get());
}

void RDSClient::DescribeDBSnapshotAttributesAsync(const DescribeDBSnapshotAttributesRequest& request, const DescribeDBSnapshotAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBSnapshotAttributes, this, request, handler, context, m_executor.get());
}
DescribeDBSnapshotsOutcome RDSClient::DescribeDBSnapshots(const DescribeDBSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBSnapshotsOutcomeCallable RDSClient::DescribeDBSnapshotsCallable(const DescribeDBSnapshotsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBSnapshots, this, request, m_executor.get());
}

void RDSClient::DescribeDBSnapshotsAsync(const DescribeDBSnapshotsRequest& request, const DescribeDBSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBSnapshots, this, request, handler, context, m_executor.get());
}
DescribeDBSubnetGroupsOutcome RDSClient::DescribeDBSubnetGroups(const DescribeDBSubnetGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDBSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDBSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDBSubnetGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDBSubnetGroupsOutcomeCallable RDSClient::DescribeDBSubnetGroupsCallable(const DescribeDBSubnetGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeDBSubnetGroups, this, request, m_executor.get());
}

void RDSClient::DescribeDBSubnetGroupsAsync(const DescribeDBSubnetGroupsRequest& request, const DescribeDBSubnetGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeDBSubnetGroups, this, request, handler, context, m_executor.get());
}
DescribeEngineDefaultClusterParametersOutcome RDSClient::DescribeEngineDefaultClusterParameters(const DescribeEngineDefaultClusterParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEngineDefaultClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEngineDefaultClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEngineDefaultClusterParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEngineDefaultClusterParametersOutcomeCallable RDSClient::DescribeEngineDefaultClusterParametersCallable(const DescribeEngineDefaultClusterParametersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeEngineDefaultClusterParameters, this, request, m_executor.get());
}

void RDSClient::DescribeEngineDefaultClusterParametersAsync(const DescribeEngineDefaultClusterParametersRequest& request, const DescribeEngineDefaultClusterParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeEngineDefaultClusterParameters, this, request, handler, context, m_executor.get());
}
DescribeEngineDefaultParametersOutcome RDSClient::DescribeEngineDefaultParameters(const DescribeEngineDefaultParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEngineDefaultParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEngineDefaultParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEngineDefaultParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEngineDefaultParametersOutcomeCallable RDSClient::DescribeEngineDefaultParametersCallable(const DescribeEngineDefaultParametersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeEngineDefaultParameters, this, request, m_executor.get());
}

void RDSClient::DescribeEngineDefaultParametersAsync(const DescribeEngineDefaultParametersRequest& request, const DescribeEngineDefaultParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeEngineDefaultParameters, this, request, handler, context, m_executor.get());
}
DescribeEventCategoriesOutcome RDSClient::DescribeEventCategories(const DescribeEventCategoriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEventCategories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEventCategories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventCategoriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventCategoriesOutcomeCallable RDSClient::DescribeEventCategoriesCallable(const DescribeEventCategoriesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeEventCategories, this, request, m_executor.get());
}

void RDSClient::DescribeEventCategoriesAsync(const DescribeEventCategoriesRequest& request, const DescribeEventCategoriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeEventCategories, this, request, handler, context, m_executor.get());
}
DescribeEventSubscriptionsOutcome RDSClient::DescribeEventSubscriptions(const DescribeEventSubscriptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEventSubscriptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEventSubscriptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventSubscriptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventSubscriptionsOutcomeCallable RDSClient::DescribeEventSubscriptionsCallable(const DescribeEventSubscriptionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeEventSubscriptions, this, request, m_executor.get());
}

void RDSClient::DescribeEventSubscriptionsAsync(const DescribeEventSubscriptionsRequest& request, const DescribeEventSubscriptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeEventSubscriptions, this, request, handler, context, m_executor.get());
}
DescribeEventsOutcome RDSClient::DescribeEvents(const DescribeEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventsOutcomeCallable RDSClient::DescribeEventsCallable(const DescribeEventsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeEvents, this, request, m_executor.get());
}

void RDSClient::DescribeEventsAsync(const DescribeEventsRequest& request, const DescribeEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeEvents, this, request, handler, context, m_executor.get());
}
DescribeExportTasksOutcome RDSClient::DescribeExportTasks(const DescribeExportTasksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeExportTasks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeExportTasks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeExportTasksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeExportTasksOutcomeCallable RDSClient::DescribeExportTasksCallable(const DescribeExportTasksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeExportTasks, this, request, m_executor.get());
}

void RDSClient::DescribeExportTasksAsync(const DescribeExportTasksRequest& request, const DescribeExportTasksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeExportTasks, this, request, handler, context, m_executor.get());
}
DescribeGlobalClustersOutcome RDSClient::DescribeGlobalClusters(const DescribeGlobalClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeGlobalClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeGlobalClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeGlobalClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeGlobalClustersOutcomeCallable RDSClient::DescribeGlobalClustersCallable(const DescribeGlobalClustersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeGlobalClusters, this, request, m_executor.get());
}

void RDSClient::DescribeGlobalClustersAsync(const DescribeGlobalClustersRequest& request, const DescribeGlobalClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeGlobalClusters, this, request, handler, context, m_executor.get());
}
DescribeOptionGroupOptionsOutcome RDSClient::DescribeOptionGroupOptions(const DescribeOptionGroupOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeOptionGroupOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeOptionGroupOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeOptionGroupOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeOptionGroupOptionsOutcomeCallable RDSClient::DescribeOptionGroupOptionsCallable(const DescribeOptionGroupOptionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeOptionGroupOptions, this, request, m_executor.get());
}

void RDSClient::DescribeOptionGroupOptionsAsync(const DescribeOptionGroupOptionsRequest& request, const DescribeOptionGroupOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeOptionGroupOptions, this, request, handler, context, m_executor.get());
}
DescribeOptionGroupsOutcome RDSClient::DescribeOptionGroups(const DescribeOptionGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeOptionGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeOptionGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeOptionGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeOptionGroupsOutcomeCallable RDSClient::DescribeOptionGroupsCallable(const DescribeOptionGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeOptionGroups, this, request, m_executor.get());
}

void RDSClient::DescribeOptionGroupsAsync(const DescribeOptionGroupsRequest& request, const DescribeOptionGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeOptionGroups, this, request, handler, context, m_executor.get());
}
DescribeOrderableDBInstanceOptionsOutcome RDSClient::DescribeOrderableDBInstanceOptions(const DescribeOrderableDBInstanceOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeOrderableDBInstanceOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeOrderableDBInstanceOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeOrderableDBInstanceOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeOrderableDBInstanceOptionsOutcomeCallable RDSClient::DescribeOrderableDBInstanceOptionsCallable(const DescribeOrderableDBInstanceOptionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeOrderableDBInstanceOptions, this, request, m_executor.get());
}

void RDSClient::DescribeOrderableDBInstanceOptionsAsync(const DescribeOrderableDBInstanceOptionsRequest& request, const DescribeOrderableDBInstanceOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeOrderableDBInstanceOptions, this, request, handler, context, m_executor.get());
}
DescribePendingMaintenanceActionsOutcome RDSClient::DescribePendingMaintenanceActions(const DescribePendingMaintenanceActionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribePendingMaintenanceActions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribePendingMaintenanceActions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribePendingMaintenanceActionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribePendingMaintenanceActionsOutcomeCallable RDSClient::DescribePendingMaintenanceActionsCallable(const DescribePendingMaintenanceActionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribePendingMaintenanceActions, this, request, m_executor.get());
}

void RDSClient::DescribePendingMaintenanceActionsAsync(const DescribePendingMaintenanceActionsRequest& request, const DescribePendingMaintenanceActionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribePendingMaintenanceActions, this, request, handler, context, m_executor.get());
}
DescribeReservedDBInstancesOutcome RDSClient::DescribeReservedDBInstances(const DescribeReservedDBInstancesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReservedDBInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReservedDBInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReservedDBInstancesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeReservedDBInstancesOutcomeCallable RDSClient::DescribeReservedDBInstancesCallable(const DescribeReservedDBInstancesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeReservedDBInstances, this, request, m_executor.get());
}

void RDSClient::DescribeReservedDBInstancesAsync(const DescribeReservedDBInstancesRequest& request, const DescribeReservedDBInstancesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeReservedDBInstances, this, request, handler, context, m_executor.get());
}
DescribeReservedDBInstancesOfferingsOutcome RDSClient::DescribeReservedDBInstancesOfferings(const DescribeReservedDBInstancesOfferingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReservedDBInstancesOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReservedDBInstancesOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReservedDBInstancesOfferingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeReservedDBInstancesOfferingsOutcomeCallable RDSClient::DescribeReservedDBInstancesOfferingsCallable(const DescribeReservedDBInstancesOfferingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeReservedDBInstancesOfferings, this, request, m_executor.get());
}

void RDSClient::DescribeReservedDBInstancesOfferingsAsync(const DescribeReservedDBInstancesOfferingsRequest& request, const DescribeReservedDBInstancesOfferingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeReservedDBInstancesOfferings, this, request, handler, context, m_executor.get());
}
DescribeSourceRegionsOutcome RDSClient::DescribeSourceRegions(const DescribeSourceRegionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSourceRegions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSourceRegions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeSourceRegionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeSourceRegionsOutcomeCallable RDSClient::DescribeSourceRegionsCallable(const DescribeSourceRegionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeSourceRegions, this, request, m_executor.get());
}

void RDSClient::DescribeSourceRegionsAsync(const DescribeSourceRegionsRequest& request, const DescribeSourceRegionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeSourceRegions, this, request, handler, context, m_executor.get());
}
DescribeValidDBInstanceModificationsOutcome RDSClient::DescribeValidDBInstanceModifications(const DescribeValidDBInstanceModificationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeValidDBInstanceModifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeValidDBInstanceModifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeValidDBInstanceModificationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeValidDBInstanceModificationsOutcomeCallable RDSClient::DescribeValidDBInstanceModificationsCallable(const DescribeValidDBInstanceModificationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DescribeValidDBInstanceModifications, this, request, m_executor.get());
}

void RDSClient::DescribeValidDBInstanceModificationsAsync(const DescribeValidDBInstanceModificationsRequest& request, const DescribeValidDBInstanceModificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DescribeValidDBInstanceModifications, this, request, handler, context, m_executor.get());
}
DownloadDBLogFilePortionOutcome RDSClient::DownloadDBLogFilePortion(const DownloadDBLogFilePortionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DownloadDBLogFilePortion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DownloadDBLogFilePortion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DownloadDBLogFilePortionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DownloadDBLogFilePortionOutcomeCallable RDSClient::DownloadDBLogFilePortionCallable(const DownloadDBLogFilePortionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::DownloadDBLogFilePortion, this, request, m_executor.get());
}

void RDSClient::DownloadDBLogFilePortionAsync(const DownloadDBLogFilePortionRequest& request, const DownloadDBLogFilePortionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::DownloadDBLogFilePortion, this, request, handler, context, m_executor.get());
}
FailoverDBClusterOutcome RDSClient::FailoverDBCluster(const FailoverDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FailoverDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FailoverDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return FailoverDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

FailoverDBClusterOutcomeCallable RDSClient::FailoverDBClusterCallable(const FailoverDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::FailoverDBCluster, this, request, m_executor.get());
}

void RDSClient::FailoverDBClusterAsync(const FailoverDBClusterRequest& request, const FailoverDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::FailoverDBCluster, this, request, handler, context, m_executor.get());
}
FailoverGlobalClusterOutcome RDSClient::FailoverGlobalCluster(const FailoverGlobalClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FailoverGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FailoverGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return FailoverGlobalClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

FailoverGlobalClusterOutcomeCallable RDSClient::FailoverGlobalClusterCallable(const FailoverGlobalClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::FailoverGlobalCluster, this, request, m_executor.get());
}

void RDSClient::FailoverGlobalClusterAsync(const FailoverGlobalClusterRequest& request, const FailoverGlobalClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::FailoverGlobalCluster, this, request, handler, context, m_executor.get());
}
ListTagsForResourceOutcome RDSClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ListTagsForResourceOutcomeCallable RDSClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ListTagsForResource, this, request, m_executor.get());
}

void RDSClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}
ModifyActivityStreamOutcome RDSClient::ModifyActivityStream(const ModifyActivityStreamRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyActivityStreamOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyActivityStreamOutcomeCallable RDSClient::ModifyActivityStreamCallable(const ModifyActivityStreamRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyActivityStream, this, request, m_executor.get());
}

void RDSClient::ModifyActivityStreamAsync(const ModifyActivityStreamRequest& request, const ModifyActivityStreamResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyActivityStream, this, request, handler, context, m_executor.get());
}
ModifyCertificatesOutcome RDSClient::ModifyCertificates(const ModifyCertificatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyCertificatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyCertificatesOutcomeCallable RDSClient::ModifyCertificatesCallable(const ModifyCertificatesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyCertificates, this, request, m_executor.get());
}

void RDSClient::ModifyCertificatesAsync(const ModifyCertificatesRequest& request, const ModifyCertificatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyCertificates, this, request, handler, context, m_executor.get());
}
ModifyCurrentDBClusterCapacityOutcome RDSClient::ModifyCurrentDBClusterCapacity(const ModifyCurrentDBClusterCapacityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyCurrentDBClusterCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyCurrentDBClusterCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyCurrentDBClusterCapacityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyCurrentDBClusterCapacityOutcomeCallable RDSClient::ModifyCurrentDBClusterCapacityCallable(const ModifyCurrentDBClusterCapacityRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyCurrentDBClusterCapacity, this, request, m_executor.get());
}

void RDSClient::ModifyCurrentDBClusterCapacityAsync(const ModifyCurrentDBClusterCapacityRequest& request, const ModifyCurrentDBClusterCapacityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyCurrentDBClusterCapacity, this, request, handler, context, m_executor.get());
}
ModifyCustomDBEngineVersionOutcome RDSClient::ModifyCustomDBEngineVersion(const ModifyCustomDBEngineVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyCustomDBEngineVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyCustomDBEngineVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyCustomDBEngineVersionOutcomeCallable RDSClient::ModifyCustomDBEngineVersionCallable(const ModifyCustomDBEngineVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyCustomDBEngineVersion, this, request, m_executor.get());
}

void RDSClient::ModifyCustomDBEngineVersionAsync(const ModifyCustomDBEngineVersionRequest& request, const ModifyCustomDBEngineVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyCustomDBEngineVersion, this, request, handler, context, m_executor.get());
}
ModifyDBClusterOutcome RDSClient::ModifyDBCluster(const ModifyDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBClusterOutcomeCallable RDSClient::ModifyDBClusterCallable(const ModifyDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBCluster, this, request, m_executor.get());
}

void RDSClient::ModifyDBClusterAsync(const ModifyDBClusterRequest& request, const ModifyDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBCluster, this, request, handler, context, m_executor.get());
}
ModifyDBClusterEndpointOutcome RDSClient::ModifyDBClusterEndpoint(const ModifyDBClusterEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBClusterEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBClusterEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBClusterEndpointOutcomeCallable RDSClient::ModifyDBClusterEndpointCallable(const ModifyDBClusterEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBClusterEndpoint, this, request, m_executor.get());
}

void RDSClient::ModifyDBClusterEndpointAsync(const ModifyDBClusterEndpointRequest& request, const ModifyDBClusterEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBClusterEndpoint, this, request, handler, context, m_executor.get());
}
ModifyDBClusterParameterGroupOutcome RDSClient::ModifyDBClusterParameterGroup(const ModifyDBClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBClusterParameterGroupOutcomeCallable RDSClient::ModifyDBClusterParameterGroupCallable(const ModifyDBClusterParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBClusterParameterGroup, this, request, m_executor.get());
}

void RDSClient::ModifyDBClusterParameterGroupAsync(const ModifyDBClusterParameterGroupRequest& request, const ModifyDBClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBClusterParameterGroup, this, request, handler, context, m_executor.get());
}
ModifyDBClusterSnapshotAttributeOutcome RDSClient::ModifyDBClusterSnapshotAttribute(const ModifyDBClusterSnapshotAttributeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBClusterSnapshotAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBClusterSnapshotAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBClusterSnapshotAttributeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBClusterSnapshotAttributeOutcomeCallable RDSClient::ModifyDBClusterSnapshotAttributeCallable(const ModifyDBClusterSnapshotAttributeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBClusterSnapshotAttribute, this, request, m_executor.get());
}

void RDSClient::ModifyDBClusterSnapshotAttributeAsync(const ModifyDBClusterSnapshotAttributeRequest& request, const ModifyDBClusterSnapshotAttributeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBClusterSnapshotAttribute, this, request, handler, context, m_executor.get());
}
ModifyDBInstanceOutcome RDSClient::ModifyDBInstance(const ModifyDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBInstanceOutcomeCallable RDSClient::ModifyDBInstanceCallable(const ModifyDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBInstance, this, request, m_executor.get());
}

void RDSClient::ModifyDBInstanceAsync(const ModifyDBInstanceRequest& request, const ModifyDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBInstance, this, request, handler, context, m_executor.get());
}
ModifyDBParameterGroupOutcome RDSClient::ModifyDBParameterGroup(const ModifyDBParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBParameterGroupOutcomeCallable RDSClient::ModifyDBParameterGroupCallable(const ModifyDBParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBParameterGroup, this, request, m_executor.get());
}

void RDSClient::ModifyDBParameterGroupAsync(const ModifyDBParameterGroupRequest& request, const ModifyDBParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBParameterGroup, this, request, handler, context, m_executor.get());
}
ModifyDBProxyOutcome RDSClient::ModifyDBProxy(const ModifyDBProxyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBProxy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBProxyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBProxyOutcomeCallable RDSClient::ModifyDBProxyCallable(const ModifyDBProxyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBProxy, this, request, m_executor.get());
}

void RDSClient::ModifyDBProxyAsync(const ModifyDBProxyRequest& request, const ModifyDBProxyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBProxy, this, request, handler, context, m_executor.get());
}
ModifyDBProxyEndpointOutcome RDSClient::ModifyDBProxyEndpoint(const ModifyDBProxyEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBProxyEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBProxyEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBProxyEndpointOutcomeCallable RDSClient::ModifyDBProxyEndpointCallable(const ModifyDBProxyEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBProxyEndpoint, this, request, m_executor.get());
}

void RDSClient::ModifyDBProxyEndpointAsync(const ModifyDBProxyEndpointRequest& request, const ModifyDBProxyEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBProxyEndpoint, this, request, handler, context, m_executor.get());
}
ModifyDBProxyTargetGroupOutcome RDSClient::ModifyDBProxyTargetGroup(const ModifyDBProxyTargetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBProxyTargetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBProxyTargetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBProxyTargetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBProxyTargetGroupOutcomeCallable RDSClient::ModifyDBProxyTargetGroupCallable(const ModifyDBProxyTargetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBProxyTargetGroup, this, request, m_executor.get());
}

void RDSClient::ModifyDBProxyTargetGroupAsync(const ModifyDBProxyTargetGroupRequest& request, const ModifyDBProxyTargetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBProxyTargetGroup, this, request, handler, context, m_executor.get());
}
ModifyDBSnapshotOutcome RDSClient::ModifyDBSnapshot(const ModifyDBSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBSnapshotOutcomeCallable RDSClient::ModifyDBSnapshotCallable(const ModifyDBSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBSnapshot, this, request, m_executor.get());
}

void RDSClient::ModifyDBSnapshotAsync(const ModifyDBSnapshotRequest& request, const ModifyDBSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBSnapshot, this, request, handler, context, m_executor.get());
}
ModifyDBSnapshotAttributeOutcome RDSClient::ModifyDBSnapshotAttribute(const ModifyDBSnapshotAttributeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBSnapshotAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBSnapshotAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBSnapshotAttributeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBSnapshotAttributeOutcomeCallable RDSClient::ModifyDBSnapshotAttributeCallable(const ModifyDBSnapshotAttributeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBSnapshotAttribute, this, request, m_executor.get());
}

void RDSClient::ModifyDBSnapshotAttributeAsync(const ModifyDBSnapshotAttributeRequest& request, const ModifyDBSnapshotAttributeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBSnapshotAttribute, this, request, handler, context, m_executor.get());
}
ModifyDBSubnetGroupOutcome RDSClient::ModifyDBSubnetGroup(const ModifyDBSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyDBSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyDBSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyDBSubnetGroupOutcomeCallable RDSClient::ModifyDBSubnetGroupCallable(const ModifyDBSubnetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyDBSubnetGroup, this, request, m_executor.get());
}

void RDSClient::ModifyDBSubnetGroupAsync(const ModifyDBSubnetGroupRequest& request, const ModifyDBSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyDBSubnetGroup, this, request, handler, context, m_executor.get());
}
ModifyEventSubscriptionOutcome RDSClient::ModifyEventSubscription(const ModifyEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyEventSubscriptionOutcomeCallable RDSClient::ModifyEventSubscriptionCallable(const ModifyEventSubscriptionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyEventSubscription, this, request, m_executor.get());
}

void RDSClient::ModifyEventSubscriptionAsync(const ModifyEventSubscriptionRequest& request, const ModifyEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyEventSubscription, this, request, handler, context, m_executor.get());
}
ModifyGlobalClusterOutcome RDSClient::ModifyGlobalCluster(const ModifyGlobalClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyGlobalClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyGlobalClusterOutcomeCallable RDSClient::ModifyGlobalClusterCallable(const ModifyGlobalClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyGlobalCluster, this, request, m_executor.get());
}

void RDSClient::ModifyGlobalClusterAsync(const ModifyGlobalClusterRequest& request, const ModifyGlobalClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyGlobalCluster, this, request, handler, context, m_executor.get());
}
ModifyOptionGroupOutcome RDSClient::ModifyOptionGroup(const ModifyOptionGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyOptionGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyOptionGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyOptionGroupOutcomeCallable RDSClient::ModifyOptionGroupCallable(const ModifyOptionGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ModifyOptionGroup, this, request, m_executor.get());
}

void RDSClient::ModifyOptionGroupAsync(const ModifyOptionGroupRequest& request, const ModifyOptionGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ModifyOptionGroup, this, request, handler, context, m_executor.get());
}
PromoteReadReplicaOutcome RDSClient::PromoteReadReplica(const PromoteReadReplicaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PromoteReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PromoteReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PromoteReadReplicaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

PromoteReadReplicaOutcomeCallable RDSClient::PromoteReadReplicaCallable(const PromoteReadReplicaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::PromoteReadReplica, this, request, m_executor.get());
}

void RDSClient::PromoteReadReplicaAsync(const PromoteReadReplicaRequest& request, const PromoteReadReplicaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::PromoteReadReplica, this, request, handler, context, m_executor.get());
}
PromoteReadReplicaDBClusterOutcome RDSClient::PromoteReadReplicaDBCluster(const PromoteReadReplicaDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PromoteReadReplicaDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PromoteReadReplicaDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PromoteReadReplicaDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

PromoteReadReplicaDBClusterOutcomeCallable RDSClient::PromoteReadReplicaDBClusterCallable(const PromoteReadReplicaDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::PromoteReadReplicaDBCluster, this, request, m_executor.get());
}

void RDSClient::PromoteReadReplicaDBClusterAsync(const PromoteReadReplicaDBClusterRequest& request, const PromoteReadReplicaDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::PromoteReadReplicaDBCluster, this, request, handler, context, m_executor.get());
}
PurchaseReservedDBInstancesOfferingOutcome RDSClient::PurchaseReservedDBInstancesOffering(const PurchaseReservedDBInstancesOfferingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PurchaseReservedDBInstancesOffering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PurchaseReservedDBInstancesOffering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PurchaseReservedDBInstancesOfferingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

PurchaseReservedDBInstancesOfferingOutcomeCallable RDSClient::PurchaseReservedDBInstancesOfferingCallable(const PurchaseReservedDBInstancesOfferingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::PurchaseReservedDBInstancesOffering, this, request, m_executor.get());
}

void RDSClient::PurchaseReservedDBInstancesOfferingAsync(const PurchaseReservedDBInstancesOfferingRequest& request, const PurchaseReservedDBInstancesOfferingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::PurchaseReservedDBInstancesOffering, this, request, handler, context, m_executor.get());
}
RebootDBClusterOutcome RDSClient::RebootDBCluster(const RebootDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RebootDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RebootDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RebootDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RebootDBClusterOutcomeCallable RDSClient::RebootDBClusterCallable(const RebootDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RebootDBCluster, this, request, m_executor.get());
}

void RDSClient::RebootDBClusterAsync(const RebootDBClusterRequest& request, const RebootDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RebootDBCluster, this, request, handler, context, m_executor.get());
}
RebootDBInstanceOutcome RDSClient::RebootDBInstance(const RebootDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RebootDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RebootDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RebootDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RebootDBInstanceOutcomeCallable RDSClient::RebootDBInstanceCallable(const RebootDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RebootDBInstance, this, request, m_executor.get());
}

void RDSClient::RebootDBInstanceAsync(const RebootDBInstanceRequest& request, const RebootDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RebootDBInstance, this, request, handler, context, m_executor.get());
}
RegisterDBProxyTargetsOutcome RDSClient::RegisterDBProxyTargets(const RegisterDBProxyTargetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterDBProxyTargets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RegisterDBProxyTargetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RegisterDBProxyTargetsOutcomeCallable RDSClient::RegisterDBProxyTargetsCallable(const RegisterDBProxyTargetsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RegisterDBProxyTargets, this, request, m_executor.get());
}

void RDSClient::RegisterDBProxyTargetsAsync(const RegisterDBProxyTargetsRequest& request, const RegisterDBProxyTargetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RegisterDBProxyTargets, this, request, handler, context, m_executor.get());
}
RemoveFromGlobalClusterOutcome RDSClient::RemoveFromGlobalCluster(const RemoveFromGlobalClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveFromGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveFromGlobalCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RemoveFromGlobalClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RemoveFromGlobalClusterOutcomeCallable RDSClient::RemoveFromGlobalClusterCallable(const RemoveFromGlobalClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RemoveFromGlobalCluster, this, request, m_executor.get());
}

void RDSClient::RemoveFromGlobalClusterAsync(const RemoveFromGlobalClusterRequest& request, const RemoveFromGlobalClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RemoveFromGlobalCluster, this, request, handler, context, m_executor.get());
}
RemoveRoleFromDBClusterOutcome RDSClient::RemoveRoleFromDBCluster(const RemoveRoleFromDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveRoleFromDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveRoleFromDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RemoveRoleFromDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RemoveRoleFromDBClusterOutcomeCallable RDSClient::RemoveRoleFromDBClusterCallable(const RemoveRoleFromDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RemoveRoleFromDBCluster, this, request, m_executor.get());
}

void RDSClient::RemoveRoleFromDBClusterAsync(const RemoveRoleFromDBClusterRequest& request, const RemoveRoleFromDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RemoveRoleFromDBCluster, this, request, handler, context, m_executor.get());
}
RemoveRoleFromDBInstanceOutcome RDSClient::RemoveRoleFromDBInstance(const RemoveRoleFromDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveRoleFromDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveRoleFromDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RemoveRoleFromDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RemoveRoleFromDBInstanceOutcomeCallable RDSClient::RemoveRoleFromDBInstanceCallable(const RemoveRoleFromDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RemoveRoleFromDBInstance, this, request, m_executor.get());
}

void RDSClient::RemoveRoleFromDBInstanceAsync(const RemoveRoleFromDBInstanceRequest& request, const RemoveRoleFromDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RemoveRoleFromDBInstance, this, request, handler, context, m_executor.get());
}
RemoveSourceIdentifierFromSubscriptionOutcome RDSClient::RemoveSourceIdentifierFromSubscription(const RemoveSourceIdentifierFromSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveSourceIdentifierFromSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveSourceIdentifierFromSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RemoveSourceIdentifierFromSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RemoveSourceIdentifierFromSubscriptionOutcomeCallable RDSClient::RemoveSourceIdentifierFromSubscriptionCallable(const RemoveSourceIdentifierFromSubscriptionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RemoveSourceIdentifierFromSubscription, this, request, m_executor.get());
}

void RDSClient::RemoveSourceIdentifierFromSubscriptionAsync(const RemoveSourceIdentifierFromSubscriptionRequest& request, const RemoveSourceIdentifierFromSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RemoveSourceIdentifierFromSubscription, this, request, handler, context, m_executor.get());
}
RemoveTagsFromResourceOutcome RDSClient::RemoveTagsFromResource(const RemoveTagsFromResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveTagsFromResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveTagsFromResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RemoveTagsFromResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RemoveTagsFromResourceOutcomeCallable RDSClient::RemoveTagsFromResourceCallable(const RemoveTagsFromResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RemoveTagsFromResource, this, request, m_executor.get());
}

void RDSClient::RemoveTagsFromResourceAsync(const RemoveTagsFromResourceRequest& request, const RemoveTagsFromResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RemoveTagsFromResource, this, request, handler, context, m_executor.get());
}
ResetDBClusterParameterGroupOutcome RDSClient::ResetDBClusterParameterGroup(const ResetDBClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResetDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResetDBClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResetDBClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ResetDBClusterParameterGroupOutcomeCallable RDSClient::ResetDBClusterParameterGroupCallable(const ResetDBClusterParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ResetDBClusterParameterGroup, this, request, m_executor.get());
}

void RDSClient::ResetDBClusterParameterGroupAsync(const ResetDBClusterParameterGroupRequest& request, const ResetDBClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ResetDBClusterParameterGroup, this, request, handler, context, m_executor.get());
}
ResetDBParameterGroupOutcome RDSClient::ResetDBParameterGroup(const ResetDBParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResetDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResetDBParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResetDBParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ResetDBParameterGroupOutcomeCallable RDSClient::ResetDBParameterGroupCallable(const ResetDBParameterGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::ResetDBParameterGroup, this, request, m_executor.get());
}

void RDSClient::ResetDBParameterGroupAsync(const ResetDBParameterGroupRequest& request, const ResetDBParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::ResetDBParameterGroup, this, request, handler, context, m_executor.get());
}
RestoreDBClusterFromS3Outcome RDSClient::RestoreDBClusterFromS3(const RestoreDBClusterFromS3Request& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBClusterFromS3, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBClusterFromS3, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBClusterFromS3Outcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBClusterFromS3OutcomeCallable RDSClient::RestoreDBClusterFromS3Callable(const RestoreDBClusterFromS3Request& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBClusterFromS3, this, request, m_executor.get());
}

void RDSClient::RestoreDBClusterFromS3Async(const RestoreDBClusterFromS3Request& request, const RestoreDBClusterFromS3ResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBClusterFromS3, this, request, handler, context, m_executor.get());
}
RestoreDBClusterFromSnapshotOutcome RDSClient::RestoreDBClusterFromSnapshot(const RestoreDBClusterFromSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBClusterFromSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBClusterFromSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBClusterFromSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBClusterFromSnapshotOutcomeCallable RDSClient::RestoreDBClusterFromSnapshotCallable(const RestoreDBClusterFromSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBClusterFromSnapshot, this, request, m_executor.get());
}

void RDSClient::RestoreDBClusterFromSnapshotAsync(const RestoreDBClusterFromSnapshotRequest& request, const RestoreDBClusterFromSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBClusterFromSnapshot, this, request, handler, context, m_executor.get());
}
RestoreDBClusterToPointInTimeOutcome RDSClient::RestoreDBClusterToPointInTime(const RestoreDBClusterToPointInTimeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBClusterToPointInTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBClusterToPointInTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBClusterToPointInTimeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBClusterToPointInTimeOutcomeCallable RDSClient::RestoreDBClusterToPointInTimeCallable(const RestoreDBClusterToPointInTimeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBClusterToPointInTime, this, request, m_executor.get());
}

void RDSClient::RestoreDBClusterToPointInTimeAsync(const RestoreDBClusterToPointInTimeRequest& request, const RestoreDBClusterToPointInTimeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBClusterToPointInTime, this, request, handler, context, m_executor.get());
}
RestoreDBInstanceFromDBSnapshotOutcome RDSClient::RestoreDBInstanceFromDBSnapshot(const RestoreDBInstanceFromDBSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBInstanceFromDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBInstanceFromDBSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBInstanceFromDBSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBInstanceFromDBSnapshotOutcomeCallable RDSClient::RestoreDBInstanceFromDBSnapshotCallable(const RestoreDBInstanceFromDBSnapshotRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBInstanceFromDBSnapshot, this, request, m_executor.get());
}

void RDSClient::RestoreDBInstanceFromDBSnapshotAsync(const RestoreDBInstanceFromDBSnapshotRequest& request, const RestoreDBInstanceFromDBSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBInstanceFromDBSnapshot, this, request, handler, context, m_executor.get());
}
RestoreDBInstanceFromS3Outcome RDSClient::RestoreDBInstanceFromS3(const RestoreDBInstanceFromS3Request& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBInstanceFromS3, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBInstanceFromS3, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBInstanceFromS3Outcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBInstanceFromS3OutcomeCallable RDSClient::RestoreDBInstanceFromS3Callable(const RestoreDBInstanceFromS3Request& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBInstanceFromS3, this, request, m_executor.get());
}

void RDSClient::RestoreDBInstanceFromS3Async(const RestoreDBInstanceFromS3Request& request, const RestoreDBInstanceFromS3ResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBInstanceFromS3, this, request, handler, context, m_executor.get());
}
RestoreDBInstanceToPointInTimeOutcome RDSClient::RestoreDBInstanceToPointInTime(const RestoreDBInstanceToPointInTimeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreDBInstanceToPointInTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreDBInstanceToPointInTime, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreDBInstanceToPointInTimeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreDBInstanceToPointInTimeOutcomeCallable RDSClient::RestoreDBInstanceToPointInTimeCallable(const RestoreDBInstanceToPointInTimeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RestoreDBInstanceToPointInTime, this, request, m_executor.get());
}

void RDSClient::RestoreDBInstanceToPointInTimeAsync(const RestoreDBInstanceToPointInTimeRequest& request, const RestoreDBInstanceToPointInTimeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RestoreDBInstanceToPointInTime, this, request, handler, context, m_executor.get());
}
RevokeDBSecurityGroupIngressOutcome RDSClient::RevokeDBSecurityGroupIngress(const RevokeDBSecurityGroupIngressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeDBSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeDBSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeDBSecurityGroupIngressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RevokeDBSecurityGroupIngressOutcomeCallable RDSClient::RevokeDBSecurityGroupIngressCallable(const RevokeDBSecurityGroupIngressRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::RevokeDBSecurityGroupIngress, this, request, m_executor.get());
}

void RDSClient::RevokeDBSecurityGroupIngressAsync(const RevokeDBSecurityGroupIngressRequest& request, const RevokeDBSecurityGroupIngressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::RevokeDBSecurityGroupIngress, this, request, handler, context, m_executor.get());
}
StartActivityStreamOutcome RDSClient::StartActivityStream(const StartActivityStreamRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartActivityStreamOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StartActivityStreamOutcomeCallable RDSClient::StartActivityStreamCallable(const StartActivityStreamRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StartActivityStream, this, request, m_executor.get());
}

void RDSClient::StartActivityStreamAsync(const StartActivityStreamRequest& request, const StartActivityStreamResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StartActivityStream, this, request, handler, context, m_executor.get());
}
StartDBClusterOutcome RDSClient::StartDBCluster(const StartDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StartDBClusterOutcomeCallable RDSClient::StartDBClusterCallable(const StartDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StartDBCluster, this, request, m_executor.get());
}

void RDSClient::StartDBClusterAsync(const StartDBClusterRequest& request, const StartDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StartDBCluster, this, request, handler, context, m_executor.get());
}
StartDBInstanceOutcome RDSClient::StartDBInstance(const StartDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StartDBInstanceOutcomeCallable RDSClient::StartDBInstanceCallable(const StartDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StartDBInstance, this, request, m_executor.get());
}

void RDSClient::StartDBInstanceAsync(const StartDBInstanceRequest& request, const StartDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StartDBInstance, this, request, handler, context, m_executor.get());
}
StartDBInstanceAutomatedBackupsReplicationOutcome RDSClient::StartDBInstanceAutomatedBackupsReplication(const StartDBInstanceAutomatedBackupsReplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartDBInstanceAutomatedBackupsReplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  StartDBInstanceAutomatedBackupsReplicationRequest newRequest = request;
  if (request.SourceRegionHasBeenSet() && !request.PreSignedUrlHasBeenSet())
  {
    Aws::Endpoint::EndpointParameters endpointParameters;
    endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", request.GetSourceRegion()));
    ResolveEndpointOutcome presignedEndpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
    AWS_OPERATION_CHECK_SUCCESS(presignedEndpointResolutionOutcome, StartDBInstanceAutomatedBackupsReplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, presignedEndpointResolutionOutcome.GetError().GetMessage());
    newRequest.SetPreSignedUrl(GeneratePresignedUrl(request, presignedEndpointResolutionOutcome.GetResult().GetURI(),
                                                    Aws::Http::HttpMethod::HTTP_GET, request.GetSourceRegion().c_str(),
                                                    {{ "DestinationRegion", m_region }}, 3600));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartDBInstanceAutomatedBackupsReplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartDBInstanceAutomatedBackupsReplicationOutcome(MakeRequest(newRequest, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StartDBInstanceAutomatedBackupsReplicationOutcomeCallable RDSClient::StartDBInstanceAutomatedBackupsReplicationCallable(const StartDBInstanceAutomatedBackupsReplicationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StartDBInstanceAutomatedBackupsReplication, this, request, m_executor.get());
}

void RDSClient::StartDBInstanceAutomatedBackupsReplicationAsync(const StartDBInstanceAutomatedBackupsReplicationRequest& request, const StartDBInstanceAutomatedBackupsReplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StartDBInstanceAutomatedBackupsReplication, this, request, handler, context, m_executor.get());
}
StartExportTaskOutcome RDSClient::StartExportTask(const StartExportTaskRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartExportTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartExportTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartExportTaskOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StartExportTaskOutcomeCallable RDSClient::StartExportTaskCallable(const StartExportTaskRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StartExportTask, this, request, m_executor.get());
}

void RDSClient::StartExportTaskAsync(const StartExportTaskRequest& request, const StartExportTaskResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StartExportTask, this, request, handler, context, m_executor.get());
}
StopActivityStreamOutcome RDSClient::StopActivityStream(const StopActivityStreamRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopActivityStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopActivityStreamOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StopActivityStreamOutcomeCallable RDSClient::StopActivityStreamCallable(const StopActivityStreamRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StopActivityStream, this, request, m_executor.get());
}

void RDSClient::StopActivityStreamAsync(const StopActivityStreamRequest& request, const StopActivityStreamResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StopActivityStream, this, request, handler, context, m_executor.get());
}
StopDBClusterOutcome RDSClient::StopDBCluster(const StopDBClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopDBCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopDBClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StopDBClusterOutcomeCallable RDSClient::StopDBClusterCallable(const StopDBClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StopDBCluster, this, request, m_executor.get());
}

void RDSClient::StopDBClusterAsync(const StopDBClusterRequest& request, const StopDBClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StopDBCluster, this, request, handler, context, m_executor.get());
}
StopDBInstanceOutcome RDSClient::StopDBInstance(const StopDBInstanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopDBInstance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopDBInstanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StopDBInstanceOutcomeCallable RDSClient::StopDBInstanceCallable(const StopDBInstanceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StopDBInstance, this, request, m_executor.get());
}

void RDSClient::StopDBInstanceAsync(const StopDBInstanceRequest& request, const StopDBInstanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StopDBInstance, this, request, handler, context, m_executor.get());
}
StopDBInstanceAutomatedBackupsReplicationOutcome RDSClient::StopDBInstanceAutomatedBackupsReplication(const StopDBInstanceAutomatedBackupsReplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopDBInstanceAutomatedBackupsReplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopDBInstanceAutomatedBackupsReplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopDBInstanceAutomatedBackupsReplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

StopDBInstanceAutomatedBackupsReplicationOutcomeCallable RDSClient::StopDBInstanceAutomatedBackupsReplicationCallable(const StopDBInstanceAutomatedBackupsReplicationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::StopDBInstanceAutomatedBackupsReplication, this, request, m_executor.get());
}

void RDSClient::StopDBInstanceAutomatedBackupsReplicationAsync(const StopDBInstanceAutomatedBackupsReplicationRequest& request, const StopDBInstanceAutomatedBackupsReplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::StopDBInstanceAutomatedBackupsReplication, this, request, handler, context, m_executor.get());
}
SwitchoverReadReplicaOutcome RDSClient::SwitchoverReadReplica(const SwitchoverReadReplicaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SwitchoverReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SwitchoverReadReplica, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SwitchoverReadReplicaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

SwitchoverReadReplicaOutcomeCallable RDSClient::SwitchoverReadReplicaCallable(const SwitchoverReadReplicaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &RDSClient::SwitchoverReadReplica, this, request, m_executor.get());
}

void RDSClient::SwitchoverReadReplicaAsync(const SwitchoverReadReplicaRequest& request, const SwitchoverReadReplicaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&RDSClient::SwitchoverReadReplica, this, request, handler, context, m_executor.get());
}
Aws::String RDSClient::GenerateConnectAuthToken(const char* dbHostName, const char* dbRegion, unsigned port, const char* dbUserName) const
{
    Aws::StringStream ss;
    ss << "http://" << dbHostName << ":" << port;
    URI uri(ss.str());
    uri.AddQueryStringParameter("Action", "connect");
    uri.AddQueryStringParameter("DBUser", dbUserName);
    auto url = GeneratePresignedUrl(uri, Aws::Http::HttpMethod::HTTP_GET, dbRegion, "rds-db", 900/*15 minutes*/);
    Aws::Utils::StringUtils::Replace(url, "http://", "");

    return url;
}
