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

#include <aws/resource-groups/ResourceGroupsClient.h>
#include <aws/resource-groups/ResourceGroupsErrorMarshaller.h>
#include <aws/resource-groups/ResourceGroupsEndpointProvider.h>
#include <aws/resource-groups/model/CreateGroupRequest.h>
#include <aws/resource-groups/model/DeleteGroupRequest.h>
#include <aws/resource-groups/model/GetGroupRequest.h>
#include <aws/resource-groups/model/GetGroupConfigurationRequest.h>
#include <aws/resource-groups/model/GetGroupQueryRequest.h>
#include <aws/resource-groups/model/GetTagsRequest.h>
#include <aws/resource-groups/model/GroupResourcesRequest.h>
#include <aws/resource-groups/model/ListGroupResourcesRequest.h>
#include <aws/resource-groups/model/ListGroupsRequest.h>
#include <aws/resource-groups/model/PutGroupConfigurationRequest.h>
#include <aws/resource-groups/model/SearchResourcesRequest.h>
#include <aws/resource-groups/model/TagRequest.h>
#include <aws/resource-groups/model/UngroupResourcesRequest.h>
#include <aws/resource-groups/model/UntagRequest.h>
#include <aws/resource-groups/model/UpdateGroupRequest.h>
#include <aws/resource-groups/model/UpdateGroupQueryRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ResourceGroups;
using namespace Aws::ResourceGroups::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ResourceGroupsClient::SERVICE_NAME = "resource-groups";
const char* ResourceGroupsClient::ALLOCATION_TAG = "ResourceGroupsClient";

ResourceGroupsClient::ResourceGroupsClient(const ResourceGroups::ResourceGroupsClientConfiguration& clientConfiguration,
                                           std::shared_ptr<ResourceGroupsEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ResourceGroupsClient::ResourceGroupsClient(const AWSCredentials& credentials,
                                           std::shared_ptr<ResourceGroupsEndpointProviderBase> endpointProvider,
                                           const ResourceGroups::ResourceGroupsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ResourceGroupsClient::ResourceGroupsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           std::shared_ptr<ResourceGroupsEndpointProviderBase> endpointProvider,
                                           const ResourceGroups::ResourceGroupsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ResourceGroupsClient::ResourceGroupsClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ResourceGroupsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ResourceGroupsClient::ResourceGroupsClient(const AWSCredentials& credentials,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ResourceGroupsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ResourceGroupsClient::ResourceGroupsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResourceGroupsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ResourceGroupsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ResourceGroupsClient::~ResourceGroupsClient()
{
}

std::shared_ptr<ResourceGroupsEndpointProviderBase>& ResourceGroupsClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ResourceGroupsClient::init(const ResourceGroups::ResourceGroupsClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Resource Groups");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ResourceGroupsClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateGroupOutcome ResourceGroupsClient::CreateGroup(const CreateGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/groups");
  return CreateGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGroupOutcomeCallable ResourceGroupsClient::CreateGroupCallable(const CreateGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::CreateGroup, this, request, m_executor.get());
}

void ResourceGroupsClient::CreateGroupAsync(const CreateGroupRequest& request, const CreateGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::CreateGroup, this, request, handler, context, m_executor.get());
}

DeleteGroupOutcome ResourceGroupsClient::DeleteGroup(const DeleteGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/delete-group");
  return DeleteGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteGroupOutcomeCallable ResourceGroupsClient::DeleteGroupCallable(const DeleteGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::DeleteGroup, this, request, m_executor.get());
}

void ResourceGroupsClient::DeleteGroupAsync(const DeleteGroupRequest& request, const DeleteGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::DeleteGroup, this, request, handler, context, m_executor.get());
}

GetGroupOutcome ResourceGroupsClient::GetGroup(const GetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-group");
  return GetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGroupOutcomeCallable ResourceGroupsClient::GetGroupCallable(const GetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::GetGroup, this, request, m_executor.get());
}

void ResourceGroupsClient::GetGroupAsync(const GetGroupRequest& request, const GetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::GetGroup, this, request, handler, context, m_executor.get());
}

GetGroupConfigurationOutcome ResourceGroupsClient::GetGroupConfiguration(const GetGroupConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGroupConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGroupConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-group-configuration");
  return GetGroupConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGroupConfigurationOutcomeCallable ResourceGroupsClient::GetGroupConfigurationCallable(const GetGroupConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::GetGroupConfiguration, this, request, m_executor.get());
}

void ResourceGroupsClient::GetGroupConfigurationAsync(const GetGroupConfigurationRequest& request, const GetGroupConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::GetGroupConfiguration, this, request, handler, context, m_executor.get());
}

GetGroupQueryOutcome ResourceGroupsClient::GetGroupQuery(const GetGroupQueryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGroupQuery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGroupQuery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-group-query");
  return GetGroupQueryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGroupQueryOutcomeCallable ResourceGroupsClient::GetGroupQueryCallable(const GetGroupQueryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::GetGroupQuery, this, request, m_executor.get());
}

void ResourceGroupsClient::GetGroupQueryAsync(const GetGroupQueryRequest& request, const GetGroupQueryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::GetGroupQuery, this, request, handler, context, m_executor.get());
}

GetTagsOutcome ResourceGroupsClient::GetTags(const GetTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTags", "Required field: Arn, is not set");
    return GetTagsOutcome(Aws::Client::AWSError<ResourceGroupsErrors>(ResourceGroupsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return GetTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTagsOutcomeCallable ResourceGroupsClient::GetTagsCallable(const GetTagsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::GetTags, this, request, m_executor.get());
}

void ResourceGroupsClient::GetTagsAsync(const GetTagsRequest& request, const GetTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::GetTags, this, request, handler, context, m_executor.get());
}

GroupResourcesOutcome ResourceGroupsClient::GroupResources(const GroupResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/group-resources");
  return GroupResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GroupResourcesOutcomeCallable ResourceGroupsClient::GroupResourcesCallable(const GroupResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::GroupResources, this, request, m_executor.get());
}

void ResourceGroupsClient::GroupResourcesAsync(const GroupResourcesRequest& request, const GroupResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::GroupResources, this, request, handler, context, m_executor.get());
}

ListGroupResourcesOutcome ResourceGroupsClient::ListGroupResources(const ListGroupResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-group-resources");
  return ListGroupResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGroupResourcesOutcomeCallable ResourceGroupsClient::ListGroupResourcesCallable(const ListGroupResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::ListGroupResources, this, request, m_executor.get());
}

void ResourceGroupsClient::ListGroupResourcesAsync(const ListGroupResourcesRequest& request, const ListGroupResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::ListGroupResources, this, request, handler, context, m_executor.get());
}

ListGroupsOutcome ResourceGroupsClient::ListGroups(const ListGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/groups-list");
  return ListGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGroupsOutcomeCallable ResourceGroupsClient::ListGroupsCallable(const ListGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::ListGroups, this, request, m_executor.get());
}

void ResourceGroupsClient::ListGroupsAsync(const ListGroupsRequest& request, const ListGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::ListGroups, this, request, handler, context, m_executor.get());
}

PutGroupConfigurationOutcome ResourceGroupsClient::PutGroupConfiguration(const PutGroupConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutGroupConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutGroupConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/put-group-configuration");
  return PutGroupConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutGroupConfigurationOutcomeCallable ResourceGroupsClient::PutGroupConfigurationCallable(const PutGroupConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::PutGroupConfiguration, this, request, m_executor.get());
}

void ResourceGroupsClient::PutGroupConfigurationAsync(const PutGroupConfigurationRequest& request, const PutGroupConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::PutGroupConfiguration, this, request, handler, context, m_executor.get());
}

SearchResourcesOutcome ResourceGroupsClient::SearchResources(const SearchResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SearchResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SearchResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/search");
  return SearchResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SearchResourcesOutcomeCallable ResourceGroupsClient::SearchResourcesCallable(const SearchResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::SearchResources, this, request, m_executor.get());
}

void ResourceGroupsClient::SearchResourcesAsync(const SearchResourcesRequest& request, const SearchResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::SearchResources, this, request, handler, context, m_executor.get());
}

TagOutcome ResourceGroupsClient::Tag(const TagRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, Tag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("Tag", "Required field: Arn, is not set");
    return TagOutcome(Aws::Client::AWSError<ResourceGroupsErrors>(ResourceGroupsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, Tag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return TagOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

TagOutcomeCallable ResourceGroupsClient::TagCallable(const TagRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::Tag, this, request, m_executor.get());
}

void ResourceGroupsClient::TagAsync(const TagRequest& request, const TagResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::Tag, this, request, handler, context, m_executor.get());
}

UngroupResourcesOutcome ResourceGroupsClient::UngroupResources(const UngroupResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UngroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UngroupResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ungroup-resources");
  return UngroupResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UngroupResourcesOutcomeCallable ResourceGroupsClient::UngroupResourcesCallable(const UngroupResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::UngroupResources, this, request, m_executor.get());
}

void ResourceGroupsClient::UngroupResourcesAsync(const UngroupResourcesRequest& request, const UngroupResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::UngroupResources, this, request, handler, context, m_executor.get());
}

UntagOutcome ResourceGroupsClient::Untag(const UntagRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, Untag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("Untag", "Required field: Arn, is not set");
    return UntagOutcome(Aws::Client::AWSError<ResourceGroupsErrors>(ResourceGroupsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, Untag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return UntagOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UntagOutcomeCallable ResourceGroupsClient::UntagCallable(const UntagRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::Untag, this, request, m_executor.get());
}

void ResourceGroupsClient::UntagAsync(const UntagRequest& request, const UntagResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::Untag, this, request, handler, context, m_executor.get());
}

UpdateGroupOutcome ResourceGroupsClient::UpdateGroup(const UpdateGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-group");
  return UpdateGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGroupOutcomeCallable ResourceGroupsClient::UpdateGroupCallable(const UpdateGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::UpdateGroup, this, request, m_executor.get());
}

void ResourceGroupsClient::UpdateGroupAsync(const UpdateGroupRequest& request, const UpdateGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::UpdateGroup, this, request, handler, context, m_executor.get());
}

UpdateGroupQueryOutcome ResourceGroupsClient::UpdateGroupQuery(const UpdateGroupQueryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGroupQuery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGroupQuery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-group-query");
  return UpdateGroupQueryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGroupQueryOutcomeCallable ResourceGroupsClient::UpdateGroupQueryCallable(const UpdateGroupQueryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResourceGroupsClient::UpdateGroupQuery, this, request, m_executor.get());
}

void ResourceGroupsClient::UpdateGroupQueryAsync(const UpdateGroupQueryRequest& request, const UpdateGroupQueryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResourceGroupsClient::UpdateGroupQuery, this, request, handler, context, m_executor.get());
}

