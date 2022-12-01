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

#include <aws/clouddirectory/CloudDirectoryClient.h>
#include <aws/clouddirectory/CloudDirectoryErrorMarshaller.h>
#include <aws/clouddirectory/CloudDirectoryEndpointProvider.h>
#include <aws/clouddirectory/model/AddFacetToObjectRequest.h>
#include <aws/clouddirectory/model/ApplySchemaRequest.h>
#include <aws/clouddirectory/model/AttachObjectRequest.h>
#include <aws/clouddirectory/model/AttachPolicyRequest.h>
#include <aws/clouddirectory/model/AttachToIndexRequest.h>
#include <aws/clouddirectory/model/AttachTypedLinkRequest.h>
#include <aws/clouddirectory/model/BatchReadRequest.h>
#include <aws/clouddirectory/model/BatchWriteRequest.h>
#include <aws/clouddirectory/model/CreateDirectoryRequest.h>
#include <aws/clouddirectory/model/CreateFacetRequest.h>
#include <aws/clouddirectory/model/CreateIndexRequest.h>
#include <aws/clouddirectory/model/CreateObjectRequest.h>
#include <aws/clouddirectory/model/CreateSchemaRequest.h>
#include <aws/clouddirectory/model/CreateTypedLinkFacetRequest.h>
#include <aws/clouddirectory/model/DeleteDirectoryRequest.h>
#include <aws/clouddirectory/model/DeleteFacetRequest.h>
#include <aws/clouddirectory/model/DeleteObjectRequest.h>
#include <aws/clouddirectory/model/DeleteSchemaRequest.h>
#include <aws/clouddirectory/model/DeleteTypedLinkFacetRequest.h>
#include <aws/clouddirectory/model/DetachFromIndexRequest.h>
#include <aws/clouddirectory/model/DetachObjectRequest.h>
#include <aws/clouddirectory/model/DetachPolicyRequest.h>
#include <aws/clouddirectory/model/DetachTypedLinkRequest.h>
#include <aws/clouddirectory/model/DisableDirectoryRequest.h>
#include <aws/clouddirectory/model/EnableDirectoryRequest.h>
#include <aws/clouddirectory/model/GetAppliedSchemaVersionRequest.h>
#include <aws/clouddirectory/model/GetDirectoryRequest.h>
#include <aws/clouddirectory/model/GetFacetRequest.h>
#include <aws/clouddirectory/model/GetLinkAttributesRequest.h>
#include <aws/clouddirectory/model/GetObjectAttributesRequest.h>
#include <aws/clouddirectory/model/GetObjectInformationRequest.h>
#include <aws/clouddirectory/model/GetSchemaAsJsonRequest.h>
#include <aws/clouddirectory/model/GetTypedLinkFacetInformationRequest.h>
#include <aws/clouddirectory/model/ListAppliedSchemaArnsRequest.h>
#include <aws/clouddirectory/model/ListAttachedIndicesRequest.h>
#include <aws/clouddirectory/model/ListDevelopmentSchemaArnsRequest.h>
#include <aws/clouddirectory/model/ListDirectoriesRequest.h>
#include <aws/clouddirectory/model/ListFacetAttributesRequest.h>
#include <aws/clouddirectory/model/ListFacetNamesRequest.h>
#include <aws/clouddirectory/model/ListIncomingTypedLinksRequest.h>
#include <aws/clouddirectory/model/ListIndexRequest.h>
#include <aws/clouddirectory/model/ListManagedSchemaArnsRequest.h>
#include <aws/clouddirectory/model/ListObjectAttributesRequest.h>
#include <aws/clouddirectory/model/ListObjectChildrenRequest.h>
#include <aws/clouddirectory/model/ListObjectParentPathsRequest.h>
#include <aws/clouddirectory/model/ListObjectParentsRequest.h>
#include <aws/clouddirectory/model/ListObjectPoliciesRequest.h>
#include <aws/clouddirectory/model/ListOutgoingTypedLinksRequest.h>
#include <aws/clouddirectory/model/ListPolicyAttachmentsRequest.h>
#include <aws/clouddirectory/model/ListPublishedSchemaArnsRequest.h>
#include <aws/clouddirectory/model/ListTagsForResourceRequest.h>
#include <aws/clouddirectory/model/ListTypedLinkFacetAttributesRequest.h>
#include <aws/clouddirectory/model/ListTypedLinkFacetNamesRequest.h>
#include <aws/clouddirectory/model/LookupPolicyRequest.h>
#include <aws/clouddirectory/model/PublishSchemaRequest.h>
#include <aws/clouddirectory/model/PutSchemaFromJsonRequest.h>
#include <aws/clouddirectory/model/RemoveFacetFromObjectRequest.h>
#include <aws/clouddirectory/model/TagResourceRequest.h>
#include <aws/clouddirectory/model/UntagResourceRequest.h>
#include <aws/clouddirectory/model/UpdateFacetRequest.h>
#include <aws/clouddirectory/model/UpdateLinkAttributesRequest.h>
#include <aws/clouddirectory/model/UpdateObjectAttributesRequest.h>
#include <aws/clouddirectory/model/UpdateSchemaRequest.h>
#include <aws/clouddirectory/model/UpdateTypedLinkFacetRequest.h>
#include <aws/clouddirectory/model/UpgradeAppliedSchemaRequest.h>
#include <aws/clouddirectory/model/UpgradePublishedSchemaRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::CloudDirectory;
using namespace Aws::CloudDirectory::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* CloudDirectoryClient::SERVICE_NAME = "clouddirectory";
const char* CloudDirectoryClient::ALLOCATION_TAG = "CloudDirectoryClient";

CloudDirectoryClient::CloudDirectoryClient(const CloudDirectory::CloudDirectoryClientConfiguration& clientConfiguration,
                                           std::shared_ptr<CloudDirectoryEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudDirectoryClient::CloudDirectoryClient(const AWSCredentials& credentials,
                                           std::shared_ptr<CloudDirectoryEndpointProviderBase> endpointProvider,
                                           const CloudDirectory::CloudDirectoryClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudDirectoryClient::CloudDirectoryClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           std::shared_ptr<CloudDirectoryEndpointProviderBase> endpointProvider,
                                           const CloudDirectory::CloudDirectoryClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  CloudDirectoryClient::CloudDirectoryClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<CloudDirectoryEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudDirectoryClient::CloudDirectoryClient(const AWSCredentials& credentials,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudDirectoryEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudDirectoryClient::CloudDirectoryClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudDirectoryErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudDirectoryEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
CloudDirectoryClient::~CloudDirectoryClient()
{
}

std::shared_ptr<CloudDirectoryEndpointProviderBase>& CloudDirectoryClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void CloudDirectoryClient::init(const CloudDirectory::CloudDirectoryClientConfiguration& config)
{
  AWSClient::SetServiceClientName("CloudDirectory");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void CloudDirectoryClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AddFacetToObjectOutcome CloudDirectoryClient::AddFacetToObject(const AddFacetToObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddFacetToObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AddFacetToObject", "Required field: DirectoryArn, is not set");
    return AddFacetToObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddFacetToObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/facets");
  return AddFacetToObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AddFacetToObjectOutcomeCallable CloudDirectoryClient::AddFacetToObjectCallable(const AddFacetToObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::AddFacetToObject, this, request, m_executor.get());
}

void CloudDirectoryClient::AddFacetToObjectAsync(const AddFacetToObjectRequest& request, const AddFacetToObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::AddFacetToObject, this, request, handler, context, m_executor.get());
}

ApplySchemaOutcome CloudDirectoryClient::ApplySchema(const ApplySchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ApplySchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ApplySchema", "Required field: DirectoryArn, is not set");
    return ApplySchemaOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ApplySchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/apply");
  return ApplySchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

ApplySchemaOutcomeCallable CloudDirectoryClient::ApplySchemaCallable(const ApplySchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ApplySchema, this, request, m_executor.get());
}

void CloudDirectoryClient::ApplySchemaAsync(const ApplySchemaRequest& request, const ApplySchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ApplySchema, this, request, handler, context, m_executor.get());
}

AttachObjectOutcome CloudDirectoryClient::AttachObject(const AttachObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AttachObject", "Required field: DirectoryArn, is not set");
    return AttachObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/attach");
  return AttachObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AttachObjectOutcomeCallable CloudDirectoryClient::AttachObjectCallable(const AttachObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::AttachObject, this, request, m_executor.get());
}

void CloudDirectoryClient::AttachObjectAsync(const AttachObjectRequest& request, const AttachObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::AttachObject, this, request, handler, context, m_executor.get());
}

AttachPolicyOutcome CloudDirectoryClient::AttachPolicy(const AttachPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AttachPolicy", "Required field: DirectoryArn, is not set");
    return AttachPolicyOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/policy/attach");
  return AttachPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AttachPolicyOutcomeCallable CloudDirectoryClient::AttachPolicyCallable(const AttachPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::AttachPolicy, this, request, m_executor.get());
}

void CloudDirectoryClient::AttachPolicyAsync(const AttachPolicyRequest& request, const AttachPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::AttachPolicy, this, request, handler, context, m_executor.get());
}

AttachToIndexOutcome CloudDirectoryClient::AttachToIndex(const AttachToIndexRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachToIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AttachToIndex", "Required field: DirectoryArn, is not set");
    return AttachToIndexOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachToIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/index/attach");
  return AttachToIndexOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AttachToIndexOutcomeCallable CloudDirectoryClient::AttachToIndexCallable(const AttachToIndexRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::AttachToIndex, this, request, m_executor.get());
}

void CloudDirectoryClient::AttachToIndexAsync(const AttachToIndexRequest& request, const AttachToIndexResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::AttachToIndex, this, request, handler, context, m_executor.get());
}

AttachTypedLinkOutcome CloudDirectoryClient::AttachTypedLink(const AttachTypedLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachTypedLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AttachTypedLink", "Required field: DirectoryArn, is not set");
    return AttachTypedLinkOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachTypedLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/attach");
  return AttachTypedLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AttachTypedLinkOutcomeCallable CloudDirectoryClient::AttachTypedLinkCallable(const AttachTypedLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::AttachTypedLink, this, request, m_executor.get());
}

void CloudDirectoryClient::AttachTypedLinkAsync(const AttachTypedLinkRequest& request, const AttachTypedLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::AttachTypedLink, this, request, handler, context, m_executor.get());
}

BatchReadOutcome CloudDirectoryClient::BatchRead(const BatchReadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchRead, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchRead", "Required field: DirectoryArn, is not set");
    return BatchReadOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchRead, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/batchread");
  return BatchReadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchReadOutcomeCallable CloudDirectoryClient::BatchReadCallable(const BatchReadRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::BatchRead, this, request, m_executor.get());
}

void CloudDirectoryClient::BatchReadAsync(const BatchReadRequest& request, const BatchReadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::BatchRead, this, request, handler, context, m_executor.get());
}

BatchWriteOutcome CloudDirectoryClient::BatchWrite(const BatchWriteRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchWrite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchWrite", "Required field: DirectoryArn, is not set");
    return BatchWriteOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchWrite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/batchwrite");
  return BatchWriteOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

BatchWriteOutcomeCallable CloudDirectoryClient::BatchWriteCallable(const BatchWriteRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::BatchWrite, this, request, m_executor.get());
}

void CloudDirectoryClient::BatchWriteAsync(const BatchWriteRequest& request, const BatchWriteResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::BatchWrite, this, request, handler, context, m_executor.get());
}

CreateDirectoryOutcome CloudDirectoryClient::CreateDirectory(const CreateDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDirectory", "Required field: SchemaArn, is not set");
    return CreateDirectoryOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory/create");
  return CreateDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateDirectoryOutcomeCallable CloudDirectoryClient::CreateDirectoryCallable(const CreateDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateDirectory, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateDirectoryAsync(const CreateDirectoryRequest& request, const CreateDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateDirectory, this, request, handler, context, m_executor.get());
}

CreateFacetOutcome CloudDirectoryClient::CreateFacet(const CreateFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateFacet", "Required field: SchemaArn, is not set");
    return CreateFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet/create");
  return CreateFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateFacetOutcomeCallable CloudDirectoryClient::CreateFacetCallable(const CreateFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateFacetAsync(const CreateFacetRequest& request, const CreateFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateFacet, this, request, handler, context, m_executor.get());
}

CreateIndexOutcome CloudDirectoryClient::CreateIndex(const CreateIndexRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateIndex", "Required field: DirectoryArn, is not set");
    return CreateIndexOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/index");
  return CreateIndexOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateIndexOutcomeCallable CloudDirectoryClient::CreateIndexCallable(const CreateIndexRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateIndex, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateIndexAsync(const CreateIndexRequest& request, const CreateIndexResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateIndex, this, request, handler, context, m_executor.get());
}

CreateObjectOutcome CloudDirectoryClient::CreateObject(const CreateObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateObject", "Required field: DirectoryArn, is not set");
    return CreateObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object");
  return CreateObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateObjectOutcomeCallable CloudDirectoryClient::CreateObjectCallable(const CreateObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateObject, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateObjectAsync(const CreateObjectRequest& request, const CreateObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateObject, this, request, handler, context, m_executor.get());
}

CreateSchemaOutcome CloudDirectoryClient::CreateSchema(const CreateSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/create");
  return CreateSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateSchemaOutcomeCallable CloudDirectoryClient::CreateSchemaCallable(const CreateSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateSchemaAsync(const CreateSchemaRequest& request, const CreateSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateSchema, this, request, handler, context, m_executor.get());
}

CreateTypedLinkFacetOutcome CloudDirectoryClient::CreateTypedLinkFacet(const CreateTypedLinkFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateTypedLinkFacet", "Required field: SchemaArn, is not set");
    return CreateTypedLinkFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet/create");
  return CreateTypedLinkFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateTypedLinkFacetOutcomeCallable CloudDirectoryClient::CreateTypedLinkFacetCallable(const CreateTypedLinkFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::CreateTypedLinkFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::CreateTypedLinkFacetAsync(const CreateTypedLinkFacetRequest& request, const CreateTypedLinkFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::CreateTypedLinkFacet, this, request, handler, context, m_executor.get());
}

DeleteDirectoryOutcome CloudDirectoryClient::DeleteDirectory(const DeleteDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDirectory", "Required field: DirectoryArn, is not set");
    return DeleteDirectoryOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory");
  return DeleteDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DeleteDirectoryOutcomeCallable CloudDirectoryClient::DeleteDirectoryCallable(const DeleteDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DeleteDirectory, this, request, m_executor.get());
}

void CloudDirectoryClient::DeleteDirectoryAsync(const DeleteDirectoryRequest& request, const DeleteDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DeleteDirectory, this, request, handler, context, m_executor.get());
}

DeleteFacetOutcome CloudDirectoryClient::DeleteFacet(const DeleteFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFacet", "Required field: SchemaArn, is not set");
    return DeleteFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet/delete");
  return DeleteFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DeleteFacetOutcomeCallable CloudDirectoryClient::DeleteFacetCallable(const DeleteFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DeleteFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::DeleteFacetAsync(const DeleteFacetRequest& request, const DeleteFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DeleteFacet, this, request, handler, context, m_executor.get());
}

DeleteObjectOutcome CloudDirectoryClient::DeleteObject(const DeleteObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteObject", "Required field: DirectoryArn, is not set");
    return DeleteObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/delete");
  return DeleteObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DeleteObjectOutcomeCallable CloudDirectoryClient::DeleteObjectCallable(const DeleteObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DeleteObject, this, request, m_executor.get());
}

void CloudDirectoryClient::DeleteObjectAsync(const DeleteObjectRequest& request, const DeleteObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DeleteObject, this, request, handler, context, m_executor.get());
}

DeleteSchemaOutcome CloudDirectoryClient::DeleteSchema(const DeleteSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSchema", "Required field: SchemaArn, is not set");
    return DeleteSchemaOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema");
  return DeleteSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DeleteSchemaOutcomeCallable CloudDirectoryClient::DeleteSchemaCallable(const DeleteSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DeleteSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::DeleteSchemaAsync(const DeleteSchemaRequest& request, const DeleteSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DeleteSchema, this, request, handler, context, m_executor.get());
}

DeleteTypedLinkFacetOutcome CloudDirectoryClient::DeleteTypedLinkFacet(const DeleteTypedLinkFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteTypedLinkFacet", "Required field: SchemaArn, is not set");
    return DeleteTypedLinkFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet/delete");
  return DeleteTypedLinkFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DeleteTypedLinkFacetOutcomeCallable CloudDirectoryClient::DeleteTypedLinkFacetCallable(const DeleteTypedLinkFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DeleteTypedLinkFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::DeleteTypedLinkFacetAsync(const DeleteTypedLinkFacetRequest& request, const DeleteTypedLinkFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DeleteTypedLinkFacet, this, request, handler, context, m_executor.get());
}

DetachFromIndexOutcome CloudDirectoryClient::DetachFromIndex(const DetachFromIndexRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachFromIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetachFromIndex", "Required field: DirectoryArn, is not set");
    return DetachFromIndexOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachFromIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/index/detach");
  return DetachFromIndexOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DetachFromIndexOutcomeCallable CloudDirectoryClient::DetachFromIndexCallable(const DetachFromIndexRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DetachFromIndex, this, request, m_executor.get());
}

void CloudDirectoryClient::DetachFromIndexAsync(const DetachFromIndexRequest& request, const DetachFromIndexResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DetachFromIndex, this, request, handler, context, m_executor.get());
}

DetachObjectOutcome CloudDirectoryClient::DetachObject(const DetachObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetachObject", "Required field: DirectoryArn, is not set");
    return DetachObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/detach");
  return DetachObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DetachObjectOutcomeCallable CloudDirectoryClient::DetachObjectCallable(const DetachObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DetachObject, this, request, m_executor.get());
}

void CloudDirectoryClient::DetachObjectAsync(const DetachObjectRequest& request, const DetachObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DetachObject, this, request, handler, context, m_executor.get());
}

DetachPolicyOutcome CloudDirectoryClient::DetachPolicy(const DetachPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetachPolicy", "Required field: DirectoryArn, is not set");
    return DetachPolicyOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/policy/detach");
  return DetachPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DetachPolicyOutcomeCallable CloudDirectoryClient::DetachPolicyCallable(const DetachPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DetachPolicy, this, request, m_executor.get());
}

void CloudDirectoryClient::DetachPolicyAsync(const DetachPolicyRequest& request, const DetachPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DetachPolicy, this, request, handler, context, m_executor.get());
}

DetachTypedLinkOutcome CloudDirectoryClient::DetachTypedLink(const DetachTypedLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachTypedLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetachTypedLink", "Required field: DirectoryArn, is not set");
    return DetachTypedLinkOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachTypedLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/detach");
  return DetachTypedLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DetachTypedLinkOutcomeCallable CloudDirectoryClient::DetachTypedLinkCallable(const DetachTypedLinkRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DetachTypedLink, this, request, m_executor.get());
}

void CloudDirectoryClient::DetachTypedLinkAsync(const DetachTypedLinkRequest& request, const DetachTypedLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DetachTypedLink, this, request, handler, context, m_executor.get());
}

DisableDirectoryOutcome CloudDirectoryClient::DisableDirectory(const DisableDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisableDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisableDirectory", "Required field: DirectoryArn, is not set");
    return DisableDirectoryOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisableDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory/disable");
  return DisableDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

DisableDirectoryOutcomeCallable CloudDirectoryClient::DisableDirectoryCallable(const DisableDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::DisableDirectory, this, request, m_executor.get());
}

void CloudDirectoryClient::DisableDirectoryAsync(const DisableDirectoryRequest& request, const DisableDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::DisableDirectory, this, request, handler, context, m_executor.get());
}

EnableDirectoryOutcome CloudDirectoryClient::EnableDirectory(const EnableDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EnableDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("EnableDirectory", "Required field: DirectoryArn, is not set");
    return EnableDirectoryOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EnableDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory/enable");
  return EnableDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

EnableDirectoryOutcomeCallable CloudDirectoryClient::EnableDirectoryCallable(const EnableDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::EnableDirectory, this, request, m_executor.get());
}

void CloudDirectoryClient::EnableDirectoryAsync(const EnableDirectoryRequest& request, const EnableDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::EnableDirectory, this, request, handler, context, m_executor.get());
}

GetAppliedSchemaVersionOutcome CloudDirectoryClient::GetAppliedSchemaVersion(const GetAppliedSchemaVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAppliedSchemaVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAppliedSchemaVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/getappliedschema");
  return GetAppliedSchemaVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetAppliedSchemaVersionOutcomeCallable CloudDirectoryClient::GetAppliedSchemaVersionCallable(const GetAppliedSchemaVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetAppliedSchemaVersion, this, request, m_executor.get());
}

void CloudDirectoryClient::GetAppliedSchemaVersionAsync(const GetAppliedSchemaVersionRequest& request, const GetAppliedSchemaVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetAppliedSchemaVersion, this, request, handler, context, m_executor.get());
}

GetDirectoryOutcome CloudDirectoryClient::GetDirectory(const GetDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDirectory", "Required field: DirectoryArn, is not set");
    return GetDirectoryOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory/get");
  return GetDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetDirectoryOutcomeCallable CloudDirectoryClient::GetDirectoryCallable(const GetDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetDirectory, this, request, m_executor.get());
}

void CloudDirectoryClient::GetDirectoryAsync(const GetDirectoryRequest& request, const GetDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetDirectory, this, request, handler, context, m_executor.get());
}

GetFacetOutcome CloudDirectoryClient::GetFacet(const GetFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetFacet", "Required field: SchemaArn, is not set");
    return GetFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet");
  return GetFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetFacetOutcomeCallable CloudDirectoryClient::GetFacetCallable(const GetFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::GetFacetAsync(const GetFacetRequest& request, const GetFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetFacet, this, request, handler, context, m_executor.get());
}

GetLinkAttributesOutcome CloudDirectoryClient::GetLinkAttributes(const GetLinkAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLinkAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetLinkAttributes", "Required field: DirectoryArn, is not set");
    return GetLinkAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLinkAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/attributes/get");
  return GetLinkAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetLinkAttributesOutcomeCallable CloudDirectoryClient::GetLinkAttributesCallable(const GetLinkAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetLinkAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::GetLinkAttributesAsync(const GetLinkAttributesRequest& request, const GetLinkAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetLinkAttributes, this, request, handler, context, m_executor.get());
}

GetObjectAttributesOutcome CloudDirectoryClient::GetObjectAttributes(const GetObjectAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetObjectAttributes", "Required field: DirectoryArn, is not set");
    return GetObjectAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/attributes/get");
  return GetObjectAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetObjectAttributesOutcomeCallable CloudDirectoryClient::GetObjectAttributesCallable(const GetObjectAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetObjectAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::GetObjectAttributesAsync(const GetObjectAttributesRequest& request, const GetObjectAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetObjectAttributes, this, request, handler, context, m_executor.get());
}

GetObjectInformationOutcome CloudDirectoryClient::GetObjectInformation(const GetObjectInformationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetObjectInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetObjectInformation", "Required field: DirectoryArn, is not set");
    return GetObjectInformationOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetObjectInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/information");
  return GetObjectInformationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetObjectInformationOutcomeCallable CloudDirectoryClient::GetObjectInformationCallable(const GetObjectInformationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetObjectInformation, this, request, m_executor.get());
}

void CloudDirectoryClient::GetObjectInformationAsync(const GetObjectInformationRequest& request, const GetObjectInformationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetObjectInformation, this, request, handler, context, m_executor.get());
}

GetSchemaAsJsonOutcome CloudDirectoryClient::GetSchemaAsJson(const GetSchemaAsJsonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSchemaAsJson, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSchemaAsJson", "Required field: SchemaArn, is not set");
    return GetSchemaAsJsonOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSchemaAsJson, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/json");
  return GetSchemaAsJsonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSchemaAsJsonOutcomeCallable CloudDirectoryClient::GetSchemaAsJsonCallable(const GetSchemaAsJsonRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetSchemaAsJson, this, request, m_executor.get());
}

void CloudDirectoryClient::GetSchemaAsJsonAsync(const GetSchemaAsJsonRequest& request, const GetSchemaAsJsonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetSchemaAsJson, this, request, handler, context, m_executor.get());
}

GetTypedLinkFacetInformationOutcome CloudDirectoryClient::GetTypedLinkFacetInformation(const GetTypedLinkFacetInformationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTypedLinkFacetInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTypedLinkFacetInformation", "Required field: SchemaArn, is not set");
    return GetTypedLinkFacetInformationOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTypedLinkFacetInformation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet/get");
  return GetTypedLinkFacetInformationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetTypedLinkFacetInformationOutcomeCallable CloudDirectoryClient::GetTypedLinkFacetInformationCallable(const GetTypedLinkFacetInformationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::GetTypedLinkFacetInformation, this, request, m_executor.get());
}

void CloudDirectoryClient::GetTypedLinkFacetInformationAsync(const GetTypedLinkFacetInformationRequest& request, const GetTypedLinkFacetInformationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::GetTypedLinkFacetInformation, this, request, handler, context, m_executor.get());
}

ListAppliedSchemaArnsOutcome CloudDirectoryClient::ListAppliedSchemaArns(const ListAppliedSchemaArnsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppliedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppliedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/applied");
  return ListAppliedSchemaArnsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppliedSchemaArnsOutcomeCallable CloudDirectoryClient::ListAppliedSchemaArnsCallable(const ListAppliedSchemaArnsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListAppliedSchemaArns, this, request, m_executor.get());
}

void CloudDirectoryClient::ListAppliedSchemaArnsAsync(const ListAppliedSchemaArnsRequest& request, const ListAppliedSchemaArnsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListAppliedSchemaArns, this, request, handler, context, m_executor.get());
}

ListAttachedIndicesOutcome CloudDirectoryClient::ListAttachedIndices(const ListAttachedIndicesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAttachedIndices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAttachedIndices", "Required field: DirectoryArn, is not set");
    return ListAttachedIndicesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAttachedIndices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/indices");
  return ListAttachedIndicesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAttachedIndicesOutcomeCallable CloudDirectoryClient::ListAttachedIndicesCallable(const ListAttachedIndicesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListAttachedIndices, this, request, m_executor.get());
}

void CloudDirectoryClient::ListAttachedIndicesAsync(const ListAttachedIndicesRequest& request, const ListAttachedIndicesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListAttachedIndices, this, request, handler, context, m_executor.get());
}

ListDevelopmentSchemaArnsOutcome CloudDirectoryClient::ListDevelopmentSchemaArns(const ListDevelopmentSchemaArnsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDevelopmentSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDevelopmentSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/development");
  return ListDevelopmentSchemaArnsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListDevelopmentSchemaArnsOutcomeCallable CloudDirectoryClient::ListDevelopmentSchemaArnsCallable(const ListDevelopmentSchemaArnsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListDevelopmentSchemaArns, this, request, m_executor.get());
}

void CloudDirectoryClient::ListDevelopmentSchemaArnsAsync(const ListDevelopmentSchemaArnsRequest& request, const ListDevelopmentSchemaArnsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListDevelopmentSchemaArns, this, request, handler, context, m_executor.get());
}

ListDirectoriesOutcome CloudDirectoryClient::ListDirectories(const ListDirectoriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDirectories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDirectories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/directory/list");
  return ListDirectoriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListDirectoriesOutcomeCallable CloudDirectoryClient::ListDirectoriesCallable(const ListDirectoriesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListDirectories, this, request, m_executor.get());
}

void CloudDirectoryClient::ListDirectoriesAsync(const ListDirectoriesRequest& request, const ListDirectoriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListDirectories, this, request, handler, context, m_executor.get());
}

ListFacetAttributesOutcome CloudDirectoryClient::ListFacetAttributes(const ListFacetAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListFacetAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListFacetAttributes", "Required field: SchemaArn, is not set");
    return ListFacetAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListFacetAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet/attributes");
  return ListFacetAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListFacetAttributesOutcomeCallable CloudDirectoryClient::ListFacetAttributesCallable(const ListFacetAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListFacetAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::ListFacetAttributesAsync(const ListFacetAttributesRequest& request, const ListFacetAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListFacetAttributes, this, request, handler, context, m_executor.get());
}

ListFacetNamesOutcome CloudDirectoryClient::ListFacetNames(const ListFacetNamesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListFacetNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListFacetNames", "Required field: SchemaArn, is not set");
    return ListFacetNamesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListFacetNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet/list");
  return ListFacetNamesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListFacetNamesOutcomeCallable CloudDirectoryClient::ListFacetNamesCallable(const ListFacetNamesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListFacetNames, this, request, m_executor.get());
}

void CloudDirectoryClient::ListFacetNamesAsync(const ListFacetNamesRequest& request, const ListFacetNamesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListFacetNames, this, request, handler, context, m_executor.get());
}

ListIncomingTypedLinksOutcome CloudDirectoryClient::ListIncomingTypedLinks(const ListIncomingTypedLinksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIncomingTypedLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListIncomingTypedLinks", "Required field: DirectoryArn, is not set");
    return ListIncomingTypedLinksOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIncomingTypedLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/incoming");
  return ListIncomingTypedLinksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListIncomingTypedLinksOutcomeCallable CloudDirectoryClient::ListIncomingTypedLinksCallable(const ListIncomingTypedLinksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListIncomingTypedLinks, this, request, m_executor.get());
}

void CloudDirectoryClient::ListIncomingTypedLinksAsync(const ListIncomingTypedLinksRequest& request, const ListIncomingTypedLinksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListIncomingTypedLinks, this, request, handler, context, m_executor.get());
}

ListIndexOutcome CloudDirectoryClient::ListIndex(const ListIndexRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListIndex", "Required field: DirectoryArn, is not set");
    return ListIndexOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIndex, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/index/targets");
  return ListIndexOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListIndexOutcomeCallable CloudDirectoryClient::ListIndexCallable(const ListIndexRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListIndex, this, request, m_executor.get());
}

void CloudDirectoryClient::ListIndexAsync(const ListIndexRequest& request, const ListIndexResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListIndex, this, request, handler, context, m_executor.get());
}

ListManagedSchemaArnsOutcome CloudDirectoryClient::ListManagedSchemaArns(const ListManagedSchemaArnsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListManagedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListManagedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/managed");
  return ListManagedSchemaArnsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListManagedSchemaArnsOutcomeCallable CloudDirectoryClient::ListManagedSchemaArnsCallable(const ListManagedSchemaArnsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListManagedSchemaArns, this, request, m_executor.get());
}

void CloudDirectoryClient::ListManagedSchemaArnsAsync(const ListManagedSchemaArnsRequest& request, const ListManagedSchemaArnsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListManagedSchemaArns, this, request, handler, context, m_executor.get());
}

ListObjectAttributesOutcome CloudDirectoryClient::ListObjectAttributes(const ListObjectAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListObjectAttributes", "Required field: DirectoryArn, is not set");
    return ListObjectAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/attributes");
  return ListObjectAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListObjectAttributesOutcomeCallable CloudDirectoryClient::ListObjectAttributesCallable(const ListObjectAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListObjectAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::ListObjectAttributesAsync(const ListObjectAttributesRequest& request, const ListObjectAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListObjectAttributes, this, request, handler, context, m_executor.get());
}

ListObjectChildrenOutcome CloudDirectoryClient::ListObjectChildren(const ListObjectChildrenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListObjectChildren, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListObjectChildren", "Required field: DirectoryArn, is not set");
    return ListObjectChildrenOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListObjectChildren, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/children");
  return ListObjectChildrenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListObjectChildrenOutcomeCallable CloudDirectoryClient::ListObjectChildrenCallable(const ListObjectChildrenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListObjectChildren, this, request, m_executor.get());
}

void CloudDirectoryClient::ListObjectChildrenAsync(const ListObjectChildrenRequest& request, const ListObjectChildrenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListObjectChildren, this, request, handler, context, m_executor.get());
}

ListObjectParentPathsOutcome CloudDirectoryClient::ListObjectParentPaths(const ListObjectParentPathsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListObjectParentPaths, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListObjectParentPaths", "Required field: DirectoryArn, is not set");
    return ListObjectParentPathsOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListObjectParentPaths, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/parentpaths");
  return ListObjectParentPathsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListObjectParentPathsOutcomeCallable CloudDirectoryClient::ListObjectParentPathsCallable(const ListObjectParentPathsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListObjectParentPaths, this, request, m_executor.get());
}

void CloudDirectoryClient::ListObjectParentPathsAsync(const ListObjectParentPathsRequest& request, const ListObjectParentPathsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListObjectParentPaths, this, request, handler, context, m_executor.get());
}

ListObjectParentsOutcome CloudDirectoryClient::ListObjectParents(const ListObjectParentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListObjectParents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListObjectParents", "Required field: DirectoryArn, is not set");
    return ListObjectParentsOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListObjectParents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/parent");
  return ListObjectParentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListObjectParentsOutcomeCallable CloudDirectoryClient::ListObjectParentsCallable(const ListObjectParentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListObjectParents, this, request, m_executor.get());
}

void CloudDirectoryClient::ListObjectParentsAsync(const ListObjectParentsRequest& request, const ListObjectParentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListObjectParents, this, request, handler, context, m_executor.get());
}

ListObjectPoliciesOutcome CloudDirectoryClient::ListObjectPolicies(const ListObjectPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListObjectPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListObjectPolicies", "Required field: DirectoryArn, is not set");
    return ListObjectPoliciesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListObjectPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/policy");
  return ListObjectPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListObjectPoliciesOutcomeCallable CloudDirectoryClient::ListObjectPoliciesCallable(const ListObjectPoliciesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListObjectPolicies, this, request, m_executor.get());
}

void CloudDirectoryClient::ListObjectPoliciesAsync(const ListObjectPoliciesRequest& request, const ListObjectPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListObjectPolicies, this, request, handler, context, m_executor.get());
}

ListOutgoingTypedLinksOutcome CloudDirectoryClient::ListOutgoingTypedLinks(const ListOutgoingTypedLinksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListOutgoingTypedLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListOutgoingTypedLinks", "Required field: DirectoryArn, is not set");
    return ListOutgoingTypedLinksOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListOutgoingTypedLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/outgoing");
  return ListOutgoingTypedLinksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListOutgoingTypedLinksOutcomeCallable CloudDirectoryClient::ListOutgoingTypedLinksCallable(const ListOutgoingTypedLinksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListOutgoingTypedLinks, this, request, m_executor.get());
}

void CloudDirectoryClient::ListOutgoingTypedLinksAsync(const ListOutgoingTypedLinksRequest& request, const ListOutgoingTypedLinksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListOutgoingTypedLinks, this, request, handler, context, m_executor.get());
}

ListPolicyAttachmentsOutcome CloudDirectoryClient::ListPolicyAttachments(const ListPolicyAttachmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPolicyAttachments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListPolicyAttachments", "Required field: DirectoryArn, is not set");
    return ListPolicyAttachmentsOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPolicyAttachments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/policy/attachment");
  return ListPolicyAttachmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPolicyAttachmentsOutcomeCallable CloudDirectoryClient::ListPolicyAttachmentsCallable(const ListPolicyAttachmentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListPolicyAttachments, this, request, m_executor.get());
}

void CloudDirectoryClient::ListPolicyAttachmentsAsync(const ListPolicyAttachmentsRequest& request, const ListPolicyAttachmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListPolicyAttachments, this, request, handler, context, m_executor.get());
}

ListPublishedSchemaArnsOutcome CloudDirectoryClient::ListPublishedSchemaArns(const ListPublishedSchemaArnsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPublishedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPublishedSchemaArns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/published");
  return ListPublishedSchemaArnsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPublishedSchemaArnsOutcomeCallable CloudDirectoryClient::ListPublishedSchemaArnsCallable(const ListPublishedSchemaArnsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListPublishedSchemaArns, this, request, m_executor.get());
}

void CloudDirectoryClient::ListPublishedSchemaArnsAsync(const ListPublishedSchemaArnsRequest& request, const ListPublishedSchemaArnsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListPublishedSchemaArns, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome CloudDirectoryClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/tags");
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable CloudDirectoryClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListTagsForResource, this, request, m_executor.get());
}

void CloudDirectoryClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

ListTypedLinkFacetAttributesOutcome CloudDirectoryClient::ListTypedLinkFacetAttributes(const ListTypedLinkFacetAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTypedLinkFacetAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTypedLinkFacetAttributes", "Required field: SchemaArn, is not set");
    return ListTypedLinkFacetAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTypedLinkFacetAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet/attributes");
  return ListTypedLinkFacetAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTypedLinkFacetAttributesOutcomeCallable CloudDirectoryClient::ListTypedLinkFacetAttributesCallable(const ListTypedLinkFacetAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListTypedLinkFacetAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::ListTypedLinkFacetAttributesAsync(const ListTypedLinkFacetAttributesRequest& request, const ListTypedLinkFacetAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListTypedLinkFacetAttributes, this, request, handler, context, m_executor.get());
}

ListTypedLinkFacetNamesOutcome CloudDirectoryClient::ListTypedLinkFacetNames(const ListTypedLinkFacetNamesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTypedLinkFacetNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTypedLinkFacetNames", "Required field: SchemaArn, is not set");
    return ListTypedLinkFacetNamesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTypedLinkFacetNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet/list");
  return ListTypedLinkFacetNamesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTypedLinkFacetNamesOutcomeCallable CloudDirectoryClient::ListTypedLinkFacetNamesCallable(const ListTypedLinkFacetNamesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::ListTypedLinkFacetNames, this, request, m_executor.get());
}

void CloudDirectoryClient::ListTypedLinkFacetNamesAsync(const ListTypedLinkFacetNamesRequest& request, const ListTypedLinkFacetNamesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::ListTypedLinkFacetNames, this, request, handler, context, m_executor.get());
}

LookupPolicyOutcome CloudDirectoryClient::LookupPolicy(const LookupPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, LookupPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("LookupPolicy", "Required field: DirectoryArn, is not set");
    return LookupPolicyOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, LookupPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/policy/lookup");
  return LookupPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

LookupPolicyOutcomeCallable CloudDirectoryClient::LookupPolicyCallable(const LookupPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::LookupPolicy, this, request, m_executor.get());
}

void CloudDirectoryClient::LookupPolicyAsync(const LookupPolicyRequest& request, const LookupPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::LookupPolicy, this, request, handler, context, m_executor.get());
}

PublishSchemaOutcome CloudDirectoryClient::PublishSchema(const PublishSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PublishSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DevelopmentSchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PublishSchema", "Required field: DevelopmentSchemaArn, is not set");
    return PublishSchemaOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DevelopmentSchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PublishSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/publish");
  return PublishSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PublishSchemaOutcomeCallable CloudDirectoryClient::PublishSchemaCallable(const PublishSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::PublishSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::PublishSchemaAsync(const PublishSchemaRequest& request, const PublishSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::PublishSchema, this, request, handler, context, m_executor.get());
}

PutSchemaFromJsonOutcome CloudDirectoryClient::PutSchemaFromJson(const PutSchemaFromJsonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutSchemaFromJson, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutSchemaFromJson", "Required field: SchemaArn, is not set");
    return PutSchemaFromJsonOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutSchemaFromJson, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/json");
  return PutSchemaFromJsonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutSchemaFromJsonOutcomeCallable CloudDirectoryClient::PutSchemaFromJsonCallable(const PutSchemaFromJsonRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::PutSchemaFromJson, this, request, m_executor.get());
}

void CloudDirectoryClient::PutSchemaFromJsonAsync(const PutSchemaFromJsonRequest& request, const PutSchemaFromJsonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::PutSchemaFromJson, this, request, handler, context, m_executor.get());
}

RemoveFacetFromObjectOutcome CloudDirectoryClient::RemoveFacetFromObject(const RemoveFacetFromObjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveFacetFromObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RemoveFacetFromObject", "Required field: DirectoryArn, is not set");
    return RemoveFacetFromObjectOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveFacetFromObject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/facets/delete");
  return RemoveFacetFromObjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

RemoveFacetFromObjectOutcomeCallable CloudDirectoryClient::RemoveFacetFromObjectCallable(const RemoveFacetFromObjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::RemoveFacetFromObject, this, request, m_executor.get());
}

void CloudDirectoryClient::RemoveFacetFromObjectAsync(const RemoveFacetFromObjectRequest& request, const RemoveFacetFromObjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::RemoveFacetFromObject, this, request, handler, context, m_executor.get());
}

TagResourceOutcome CloudDirectoryClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/tags/add");
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable CloudDirectoryClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::TagResource, this, request, m_executor.get());
}

void CloudDirectoryClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome CloudDirectoryClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/tags/remove");
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable CloudDirectoryClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UntagResource, this, request, m_executor.get());
}

void CloudDirectoryClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateFacetOutcome CloudDirectoryClient::UpdateFacet(const UpdateFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateFacet", "Required field: SchemaArn, is not set");
    return UpdateFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/facet");
  return UpdateFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateFacetOutcomeCallable CloudDirectoryClient::UpdateFacetCallable(const UpdateFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpdateFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::UpdateFacetAsync(const UpdateFacetRequest& request, const UpdateFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpdateFacet, this, request, handler, context, m_executor.get());
}

UpdateLinkAttributesOutcome CloudDirectoryClient::UpdateLinkAttributes(const UpdateLinkAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateLinkAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateLinkAttributes", "Required field: DirectoryArn, is not set");
    return UpdateLinkAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateLinkAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/attributes/update");
  return UpdateLinkAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateLinkAttributesOutcomeCallable CloudDirectoryClient::UpdateLinkAttributesCallable(const UpdateLinkAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpdateLinkAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::UpdateLinkAttributesAsync(const UpdateLinkAttributesRequest& request, const UpdateLinkAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpdateLinkAttributes, this, request, handler, context, m_executor.get());
}

UpdateObjectAttributesOutcome CloudDirectoryClient::UpdateObjectAttributes(const UpdateObjectAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DirectoryArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateObjectAttributes", "Required field: DirectoryArn, is not set");
    return UpdateObjectAttributesOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DirectoryArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateObjectAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/object/update");
  return UpdateObjectAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateObjectAttributesOutcomeCallable CloudDirectoryClient::UpdateObjectAttributesCallable(const UpdateObjectAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpdateObjectAttributes, this, request, m_executor.get());
}

void CloudDirectoryClient::UpdateObjectAttributesAsync(const UpdateObjectAttributesRequest& request, const UpdateObjectAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpdateObjectAttributes, this, request, handler, context, m_executor.get());
}

UpdateSchemaOutcome CloudDirectoryClient::UpdateSchema(const UpdateSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateSchema", "Required field: SchemaArn, is not set");
    return UpdateSchemaOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/update");
  return UpdateSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateSchemaOutcomeCallable CloudDirectoryClient::UpdateSchemaCallable(const UpdateSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpdateSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::UpdateSchemaAsync(const UpdateSchemaRequest& request, const UpdateSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpdateSchema, this, request, handler, context, m_executor.get());
}

UpdateTypedLinkFacetOutcome CloudDirectoryClient::UpdateTypedLinkFacet(const UpdateTypedLinkFacetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SchemaArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateTypedLinkFacet", "Required field: SchemaArn, is not set");
    return UpdateTypedLinkFacetOutcome(Aws::Client::AWSError<CloudDirectoryErrors>(CloudDirectoryErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SchemaArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateTypedLinkFacet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/typedlink/facet");
  return UpdateTypedLinkFacetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateTypedLinkFacetOutcomeCallable CloudDirectoryClient::UpdateTypedLinkFacetCallable(const UpdateTypedLinkFacetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpdateTypedLinkFacet, this, request, m_executor.get());
}

void CloudDirectoryClient::UpdateTypedLinkFacetAsync(const UpdateTypedLinkFacetRequest& request, const UpdateTypedLinkFacetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpdateTypedLinkFacet, this, request, handler, context, m_executor.get());
}

UpgradeAppliedSchemaOutcome CloudDirectoryClient::UpgradeAppliedSchema(const UpgradeAppliedSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpgradeAppliedSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpgradeAppliedSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/upgradeapplied");
  return UpgradeAppliedSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpgradeAppliedSchemaOutcomeCallable CloudDirectoryClient::UpgradeAppliedSchemaCallable(const UpgradeAppliedSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpgradeAppliedSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::UpgradeAppliedSchemaAsync(const UpgradeAppliedSchemaRequest& request, const UpgradeAppliedSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpgradeAppliedSchema, this, request, handler, context, m_executor.get());
}

UpgradePublishedSchemaOutcome CloudDirectoryClient::UpgradePublishedSchema(const UpgradePublishedSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpgradePublishedSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpgradePublishedSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/amazonclouddirectory/2017-01-11/schema/upgradepublished");
  return UpgradePublishedSchemaOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpgradePublishedSchemaOutcomeCallable CloudDirectoryClient::UpgradePublishedSchemaCallable(const UpgradePublishedSchemaRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudDirectoryClient::UpgradePublishedSchema, this, request, m_executor.get());
}

void CloudDirectoryClient::UpgradePublishedSchemaAsync(const UpgradePublishedSchemaRequest& request, const UpgradePublishedSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudDirectoryClient::UpgradePublishedSchema, this, request, handler, context, m_executor.get());
}

