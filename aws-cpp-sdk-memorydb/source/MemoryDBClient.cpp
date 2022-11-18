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
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/memorydb/MemoryDBClient.h>
#include <aws/memorydb/MemoryDBErrorMarshaller.h>
#include <aws/memorydb/MemoryDBEndpointProvider.h>
#include <aws/memorydb/model/BatchUpdateClusterRequest.h>
#include <aws/memorydb/model/CopySnapshotRequest.h>
#include <aws/memorydb/model/CreateACLRequest.h>
#include <aws/memorydb/model/CreateClusterRequest.h>
#include <aws/memorydb/model/CreateParameterGroupRequest.h>
#include <aws/memorydb/model/CreateSnapshotRequest.h>
#include <aws/memorydb/model/CreateSubnetGroupRequest.h>
#include <aws/memorydb/model/CreateUserRequest.h>
#include <aws/memorydb/model/DeleteACLRequest.h>
#include <aws/memorydb/model/DeleteClusterRequest.h>
#include <aws/memorydb/model/DeleteParameterGroupRequest.h>
#include <aws/memorydb/model/DeleteSnapshotRequest.h>
#include <aws/memorydb/model/DeleteSubnetGroupRequest.h>
#include <aws/memorydb/model/DeleteUserRequest.h>
#include <aws/memorydb/model/DescribeACLsRequest.h>
#include <aws/memorydb/model/DescribeClustersRequest.h>
#include <aws/memorydb/model/DescribeEngineVersionsRequest.h>
#include <aws/memorydb/model/DescribeEventsRequest.h>
#include <aws/memorydb/model/DescribeParameterGroupsRequest.h>
#include <aws/memorydb/model/DescribeParametersRequest.h>
#include <aws/memorydb/model/DescribeServiceUpdatesRequest.h>
#include <aws/memorydb/model/DescribeSnapshotsRequest.h>
#include <aws/memorydb/model/DescribeSubnetGroupsRequest.h>
#include <aws/memorydb/model/DescribeUsersRequest.h>
#include <aws/memorydb/model/FailoverShardRequest.h>
#include <aws/memorydb/model/ListAllowedNodeTypeUpdatesRequest.h>
#include <aws/memorydb/model/ListTagsRequest.h>
#include <aws/memorydb/model/ResetParameterGroupRequest.h>
#include <aws/memorydb/model/TagResourceRequest.h>
#include <aws/memorydb/model/UntagResourceRequest.h>
#include <aws/memorydb/model/UpdateACLRequest.h>
#include <aws/memorydb/model/UpdateClusterRequest.h>
#include <aws/memorydb/model/UpdateParameterGroupRequest.h>
#include <aws/memorydb/model/UpdateSubnetGroupRequest.h>
#include <aws/memorydb/model/UpdateUserRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::MemoryDB;
using namespace Aws::MemoryDB::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* MemoryDBClient::SERVICE_NAME = "memorydb";
const char* MemoryDBClient::ALLOCATION_TAG = "MemoryDBClient";

MemoryDBClient::MemoryDBClient(const MemoryDB::MemoryDBClientConfiguration& clientConfiguration,
                               std::shared_ptr<MemoryDBEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MemoryDBClient::MemoryDBClient(const AWSCredentials& credentials,
                               std::shared_ptr<MemoryDBEndpointProviderBase> endpointProvider,
                               const MemoryDB::MemoryDBClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MemoryDBClient::MemoryDBClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<MemoryDBEndpointProviderBase> endpointProvider,
                               const MemoryDB::MemoryDBClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  MemoryDBClient::MemoryDBClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<MemoryDBEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MemoryDBClient::MemoryDBClient(const AWSCredentials& credentials,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MemoryDBEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MemoryDBClient::MemoryDBClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MemoryDBErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MemoryDBEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
MemoryDBClient::~MemoryDBClient()
{
}

std::shared_ptr<MemoryDBEndpointProviderBase>& MemoryDBClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void MemoryDBClient::init(const MemoryDB::MemoryDBClientConfiguration& config)
{
  AWSClient::SetServiceClientName("MemoryDB");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void MemoryDBClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchUpdateClusterOutcome MemoryDBClient::BatchUpdateCluster(const BatchUpdateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchUpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchUpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return BatchUpdateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchUpdateClusterOutcomeCallable MemoryDBClient::BatchUpdateClusterCallable(const BatchUpdateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchUpdateCluster, request, m_executor.get());
}

void MemoryDBClient::BatchUpdateClusterAsync(const BatchUpdateClusterRequest& request, const BatchUpdateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchUpdateCluster, request, handler, context, m_executor.get());
}

CopySnapshotOutcome MemoryDBClient::CopySnapshot(const CopySnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopySnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopySnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopySnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CopySnapshotOutcomeCallable MemoryDBClient::CopySnapshotCallable(const CopySnapshotRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CopySnapshot, request, m_executor.get());
}

void MemoryDBClient::CopySnapshotAsync(const CopySnapshotRequest& request, const CopySnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CopySnapshot, request, handler, context, m_executor.get());
}

CreateACLOutcome MemoryDBClient::CreateACL(const CreateACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateACLOutcomeCallable MemoryDBClient::CreateACLCallable(const CreateACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateACL, request, m_executor.get());
}

void MemoryDBClient::CreateACLAsync(const CreateACLRequest& request, const CreateACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateACL, request, handler, context, m_executor.get());
}

CreateClusterOutcome MemoryDBClient::CreateCluster(const CreateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateClusterOutcomeCallable MemoryDBClient::CreateClusterCallable(const CreateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateCluster, request, m_executor.get());
}

void MemoryDBClient::CreateClusterAsync(const CreateClusterRequest& request, const CreateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateCluster, request, handler, context, m_executor.get());
}

CreateParameterGroupOutcome MemoryDBClient::CreateParameterGroup(const CreateParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateParameterGroupOutcomeCallable MemoryDBClient::CreateParameterGroupCallable(const CreateParameterGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateParameterGroup, request, m_executor.get());
}

void MemoryDBClient::CreateParameterGroupAsync(const CreateParameterGroupRequest& request, const CreateParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateParameterGroup, request, handler, context, m_executor.get());
}

CreateSnapshotOutcome MemoryDBClient::CreateSnapshot(const CreateSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSnapshotOutcomeCallable MemoryDBClient::CreateSnapshotCallable(const CreateSnapshotRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateSnapshot, request, m_executor.get());
}

void MemoryDBClient::CreateSnapshotAsync(const CreateSnapshotRequest& request, const CreateSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateSnapshot, request, handler, context, m_executor.get());
}

CreateSubnetGroupOutcome MemoryDBClient::CreateSubnetGroup(const CreateSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSubnetGroupOutcomeCallable MemoryDBClient::CreateSubnetGroupCallable(const CreateSubnetGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateSubnetGroup, request, m_executor.get());
}

void MemoryDBClient::CreateSubnetGroupAsync(const CreateSubnetGroupRequest& request, const CreateSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateSubnetGroup, request, handler, context, m_executor.get());
}

CreateUserOutcome MemoryDBClient::CreateUser(const CreateUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUserOutcomeCallable MemoryDBClient::CreateUserCallable(const CreateUserRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateUser, request, m_executor.get());
}

void MemoryDBClient::CreateUserAsync(const CreateUserRequest& request, const CreateUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateUser, request, handler, context, m_executor.get());
}

DeleteACLOutcome MemoryDBClient::DeleteACL(const DeleteACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteACLOutcomeCallable MemoryDBClient::DeleteACLCallable(const DeleteACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteACL, request, m_executor.get());
}

void MemoryDBClient::DeleteACLAsync(const DeleteACLRequest& request, const DeleteACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteACL, request, handler, context, m_executor.get());
}

DeleteClusterOutcome MemoryDBClient::DeleteCluster(const DeleteClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteClusterOutcomeCallable MemoryDBClient::DeleteClusterCallable(const DeleteClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteCluster, request, m_executor.get());
}

void MemoryDBClient::DeleteClusterAsync(const DeleteClusterRequest& request, const DeleteClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteCluster, request, handler, context, m_executor.get());
}

DeleteParameterGroupOutcome MemoryDBClient::DeleteParameterGroup(const DeleteParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteParameterGroupOutcomeCallable MemoryDBClient::DeleteParameterGroupCallable(const DeleteParameterGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteParameterGroup, request, m_executor.get());
}

void MemoryDBClient::DeleteParameterGroupAsync(const DeleteParameterGroupRequest& request, const DeleteParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteParameterGroup, request, handler, context, m_executor.get());
}

DeleteSnapshotOutcome MemoryDBClient::DeleteSnapshot(const DeleteSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSnapshotOutcomeCallable MemoryDBClient::DeleteSnapshotCallable(const DeleteSnapshotRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteSnapshot, request, m_executor.get());
}

void MemoryDBClient::DeleteSnapshotAsync(const DeleteSnapshotRequest& request, const DeleteSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteSnapshot, request, handler, context, m_executor.get());
}

DeleteSubnetGroupOutcome MemoryDBClient::DeleteSubnetGroup(const DeleteSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSubnetGroupOutcomeCallable MemoryDBClient::DeleteSubnetGroupCallable(const DeleteSubnetGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteSubnetGroup, request, m_executor.get());
}

void MemoryDBClient::DeleteSubnetGroupAsync(const DeleteSubnetGroupRequest& request, const DeleteSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteSubnetGroup, request, handler, context, m_executor.get());
}

DeleteUserOutcome MemoryDBClient::DeleteUser(const DeleteUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteUserOutcomeCallable MemoryDBClient::DeleteUserCallable(const DeleteUserRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteUser, request, m_executor.get());
}

void MemoryDBClient::DeleteUserAsync(const DeleteUserRequest& request, const DeleteUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteUser, request, handler, context, m_executor.get());
}

DescribeACLsOutcome MemoryDBClient::DescribeACLs(const DescribeACLsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeACLsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeACLsOutcomeCallable MemoryDBClient::DescribeACLsCallable(const DescribeACLsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeACLs, request, m_executor.get());
}

void MemoryDBClient::DescribeACLsAsync(const DescribeACLsRequest& request, const DescribeACLsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeACLs, request, handler, context, m_executor.get());
}

DescribeClustersOutcome MemoryDBClient::DescribeClusters(const DescribeClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeClustersOutcomeCallable MemoryDBClient::DescribeClustersCallable(const DescribeClustersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeClusters, request, m_executor.get());
}

void MemoryDBClient::DescribeClustersAsync(const DescribeClustersRequest& request, const DescribeClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeClusters, request, handler, context, m_executor.get());
}

DescribeEngineVersionsOutcome MemoryDBClient::DescribeEngineVersions(const DescribeEngineVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEngineVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEngineVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEngineVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeEngineVersionsOutcomeCallable MemoryDBClient::DescribeEngineVersionsCallable(const DescribeEngineVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeEngineVersions, request, m_executor.get());
}

void MemoryDBClient::DescribeEngineVersionsAsync(const DescribeEngineVersionsRequest& request, const DescribeEngineVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeEngineVersions, request, handler, context, m_executor.get());
}

DescribeEventsOutcome MemoryDBClient::DescribeEvents(const DescribeEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeEventsOutcomeCallable MemoryDBClient::DescribeEventsCallable(const DescribeEventsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeEvents, request, m_executor.get());
}

void MemoryDBClient::DescribeEventsAsync(const DescribeEventsRequest& request, const DescribeEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeEvents, request, handler, context, m_executor.get());
}

DescribeParameterGroupsOutcome MemoryDBClient::DescribeParameterGroups(const DescribeParameterGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeParameterGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeParameterGroupsOutcomeCallable MemoryDBClient::DescribeParameterGroupsCallable(const DescribeParameterGroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeParameterGroups, request, m_executor.get());
}

void MemoryDBClient::DescribeParameterGroupsAsync(const DescribeParameterGroupsRequest& request, const DescribeParameterGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeParameterGroups, request, handler, context, m_executor.get());
}

DescribeParametersOutcome MemoryDBClient::DescribeParameters(const DescribeParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeParametersOutcomeCallable MemoryDBClient::DescribeParametersCallable(const DescribeParametersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeParameters, request, m_executor.get());
}

void MemoryDBClient::DescribeParametersAsync(const DescribeParametersRequest& request, const DescribeParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeParameters, request, handler, context, m_executor.get());
}

DescribeServiceUpdatesOutcome MemoryDBClient::DescribeServiceUpdates(const DescribeServiceUpdatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeServiceUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeServiceUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeServiceUpdatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeServiceUpdatesOutcomeCallable MemoryDBClient::DescribeServiceUpdatesCallable(const DescribeServiceUpdatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeServiceUpdates, request, m_executor.get());
}

void MemoryDBClient::DescribeServiceUpdatesAsync(const DescribeServiceUpdatesRequest& request, const DescribeServiceUpdatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeServiceUpdates, request, handler, context, m_executor.get());
}

DescribeSnapshotsOutcome MemoryDBClient::DescribeSnapshots(const DescribeSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSnapshotsOutcomeCallable MemoryDBClient::DescribeSnapshotsCallable(const DescribeSnapshotsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeSnapshots, request, m_executor.get());
}

void MemoryDBClient::DescribeSnapshotsAsync(const DescribeSnapshotsRequest& request, const DescribeSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeSnapshots, request, handler, context, m_executor.get());
}

DescribeSubnetGroupsOutcome MemoryDBClient::DescribeSubnetGroups(const DescribeSubnetGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeSubnetGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSubnetGroupsOutcomeCallable MemoryDBClient::DescribeSubnetGroupsCallable(const DescribeSubnetGroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeSubnetGroups, request, m_executor.get());
}

void MemoryDBClient::DescribeSubnetGroupsAsync(const DescribeSubnetGroupsRequest& request, const DescribeSubnetGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeSubnetGroups, request, handler, context, m_executor.get());
}

DescribeUsersOutcome MemoryDBClient::DescribeUsers(const DescribeUsersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUsers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUsers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUsersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeUsersOutcomeCallable MemoryDBClient::DescribeUsersCallable(const DescribeUsersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeUsers, request, m_executor.get());
}

void MemoryDBClient::DescribeUsersAsync(const DescribeUsersRequest& request, const DescribeUsersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeUsers, request, handler, context, m_executor.get());
}

FailoverShardOutcome MemoryDBClient::FailoverShard(const FailoverShardRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FailoverShard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FailoverShard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return FailoverShardOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

FailoverShardOutcomeCallable MemoryDBClient::FailoverShardCallable(const FailoverShardRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(FailoverShard, request, m_executor.get());
}

void MemoryDBClient::FailoverShardAsync(const FailoverShardRequest& request, const FailoverShardResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(FailoverShard, request, handler, context, m_executor.get());
}

ListAllowedNodeTypeUpdatesOutcome MemoryDBClient::ListAllowedNodeTypeUpdates(const ListAllowedNodeTypeUpdatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAllowedNodeTypeUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAllowedNodeTypeUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAllowedNodeTypeUpdatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAllowedNodeTypeUpdatesOutcomeCallable MemoryDBClient::ListAllowedNodeTypeUpdatesCallable(const ListAllowedNodeTypeUpdatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAllowedNodeTypeUpdates, request, m_executor.get());
}

void MemoryDBClient::ListAllowedNodeTypeUpdatesAsync(const ListAllowedNodeTypeUpdatesRequest& request, const ListAllowedNodeTypeUpdatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAllowedNodeTypeUpdates, request, handler, context, m_executor.get());
}

ListTagsOutcome MemoryDBClient::ListTags(const ListTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsOutcomeCallable MemoryDBClient::ListTagsCallable(const ListTagsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTags, request, m_executor.get());
}

void MemoryDBClient::ListTagsAsync(const ListTagsRequest& request, const ListTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTags, request, handler, context, m_executor.get());
}

ResetParameterGroupOutcome MemoryDBClient::ResetParameterGroup(const ResetParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResetParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResetParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResetParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ResetParameterGroupOutcomeCallable MemoryDBClient::ResetParameterGroupCallable(const ResetParameterGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ResetParameterGroup, request, m_executor.get());
}

void MemoryDBClient::ResetParameterGroupAsync(const ResetParameterGroupRequest& request, const ResetParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ResetParameterGroup, request, handler, context, m_executor.get());
}

TagResourceOutcome MemoryDBClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable MemoryDBClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void MemoryDBClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome MemoryDBClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable MemoryDBClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void MemoryDBClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateACLOutcome MemoryDBClient::UpdateACL(const UpdateACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateACLOutcomeCallable MemoryDBClient::UpdateACLCallable(const UpdateACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateACL, request, m_executor.get());
}

void MemoryDBClient::UpdateACLAsync(const UpdateACLRequest& request, const UpdateACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateACL, request, handler, context, m_executor.get());
}

UpdateClusterOutcome MemoryDBClient::UpdateCluster(const UpdateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateClusterOutcomeCallable MemoryDBClient::UpdateClusterCallable(const UpdateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCluster, request, m_executor.get());
}

void MemoryDBClient::UpdateClusterAsync(const UpdateClusterRequest& request, const UpdateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCluster, request, handler, context, m_executor.get());
}

UpdateParameterGroupOutcome MemoryDBClient::UpdateParameterGroup(const UpdateParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateParameterGroupOutcomeCallable MemoryDBClient::UpdateParameterGroupCallable(const UpdateParameterGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateParameterGroup, request, m_executor.get());
}

void MemoryDBClient::UpdateParameterGroupAsync(const UpdateParameterGroupRequest& request, const UpdateParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateParameterGroup, request, handler, context, m_executor.get());
}

UpdateSubnetGroupOutcome MemoryDBClient::UpdateSubnetGroup(const UpdateSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateSubnetGroupOutcomeCallable MemoryDBClient::UpdateSubnetGroupCallable(const UpdateSubnetGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateSubnetGroup, request, m_executor.get());
}

void MemoryDBClient::UpdateSubnetGroupAsync(const UpdateSubnetGroupRequest& request, const UpdateSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateSubnetGroup, request, handler, context, m_executor.get());
}

UpdateUserOutcome MemoryDBClient::UpdateUser(const UpdateUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateUserOutcomeCallable MemoryDBClient::UpdateUserCallable(const UpdateUserRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateUser, request, m_executor.get());
}

void MemoryDBClient::UpdateUserAsync(const UpdateUserRequest& request, const UpdateUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateUser, request, handler, context, m_executor.get());
}

