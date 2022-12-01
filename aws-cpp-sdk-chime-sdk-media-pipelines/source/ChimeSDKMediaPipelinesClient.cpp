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

#include <aws/chime-sdk-media-pipelines/ChimeSDKMediaPipelinesClient.h>
#include <aws/chime-sdk-media-pipelines/ChimeSDKMediaPipelinesErrorMarshaller.h>
#include <aws/chime-sdk-media-pipelines/ChimeSDKMediaPipelinesEndpointProvider.h>
#include <aws/chime-sdk-media-pipelines/model/CreateMediaCapturePipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/CreateMediaConcatenationPipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/CreateMediaLiveConnectorPipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/DeleteMediaCapturePipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/DeleteMediaPipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/GetMediaCapturePipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/GetMediaPipelineRequest.h>
#include <aws/chime-sdk-media-pipelines/model/ListMediaCapturePipelinesRequest.h>
#include <aws/chime-sdk-media-pipelines/model/ListMediaPipelinesRequest.h>
#include <aws/chime-sdk-media-pipelines/model/ListTagsForResourceRequest.h>
#include <aws/chime-sdk-media-pipelines/model/TagResourceRequest.h>
#include <aws/chime-sdk-media-pipelines/model/UntagResourceRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ChimeSDKMediaPipelines;
using namespace Aws::ChimeSDKMediaPipelines::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ChimeSDKMediaPipelinesClient::SERVICE_NAME = "chime";
const char* ChimeSDKMediaPipelinesClient::ALLOCATION_TAG = "ChimeSDKMediaPipelinesClient";

ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const ChimeSDKMediaPipelines::ChimeSDKMediaPipelinesClientConfiguration& clientConfiguration,
                                                           std::shared_ptr<ChimeSDKMediaPipelinesEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const AWSCredentials& credentials,
                                                           std::shared_ptr<ChimeSDKMediaPipelinesEndpointProviderBase> endpointProvider,
                                                           const ChimeSDKMediaPipelines::ChimeSDKMediaPipelinesClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                           std::shared_ptr<ChimeSDKMediaPipelinesEndpointProviderBase> endpointProvider,
                                                           const ChimeSDKMediaPipelines::ChimeSDKMediaPipelinesClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ChimeSDKMediaPipelinesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const AWSCredentials& credentials,
                                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMediaPipelinesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMediaPipelinesClient::ChimeSDKMediaPipelinesClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMediaPipelinesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMediaPipelinesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ChimeSDKMediaPipelinesClient::~ChimeSDKMediaPipelinesClient()
{
}

std::shared_ptr<ChimeSDKMediaPipelinesEndpointProviderBase>& ChimeSDKMediaPipelinesClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ChimeSDKMediaPipelinesClient::init(const ChimeSDKMediaPipelines::ChimeSDKMediaPipelinesClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Chime SDK Media Pipelines");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ChimeSDKMediaPipelinesClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateMediaCapturePipelineOutcome ChimeSDKMediaPipelinesClient::CreateMediaCapturePipeline(const CreateMediaCapturePipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-capture-pipelines");
  return CreateMediaCapturePipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMediaCapturePipelineOutcomeCallable ChimeSDKMediaPipelinesClient::CreateMediaCapturePipelineCallable(const CreateMediaCapturePipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::CreateMediaCapturePipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::CreateMediaCapturePipelineAsync(const CreateMediaCapturePipelineRequest& request, const CreateMediaCapturePipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::CreateMediaCapturePipeline, this, request, handler, context, m_executor.get());
}

CreateMediaConcatenationPipelineOutcome ChimeSDKMediaPipelinesClient::CreateMediaConcatenationPipeline(const CreateMediaConcatenationPipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMediaConcatenationPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMediaConcatenationPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-concatenation-pipelines");
  return CreateMediaConcatenationPipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMediaConcatenationPipelineOutcomeCallable ChimeSDKMediaPipelinesClient::CreateMediaConcatenationPipelineCallable(const CreateMediaConcatenationPipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::CreateMediaConcatenationPipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::CreateMediaConcatenationPipelineAsync(const CreateMediaConcatenationPipelineRequest& request, const CreateMediaConcatenationPipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::CreateMediaConcatenationPipeline, this, request, handler, context, m_executor.get());
}

CreateMediaLiveConnectorPipelineOutcome ChimeSDKMediaPipelinesClient::CreateMediaLiveConnectorPipeline(const CreateMediaLiveConnectorPipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMediaLiveConnectorPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMediaLiveConnectorPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-live-connector-pipelines");
  return CreateMediaLiveConnectorPipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMediaLiveConnectorPipelineOutcomeCallable ChimeSDKMediaPipelinesClient::CreateMediaLiveConnectorPipelineCallable(const CreateMediaLiveConnectorPipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::CreateMediaLiveConnectorPipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::CreateMediaLiveConnectorPipelineAsync(const CreateMediaLiveConnectorPipelineRequest& request, const CreateMediaLiveConnectorPipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::CreateMediaLiveConnectorPipeline, this, request, handler, context, m_executor.get());
}

DeleteMediaCapturePipelineOutcome ChimeSDKMediaPipelinesClient::DeleteMediaCapturePipeline(const DeleteMediaCapturePipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MediaPipelineIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMediaCapturePipeline", "Required field: MediaPipelineId, is not set");
    return DeleteMediaCapturePipelineOutcome(Aws::Client::AWSError<ChimeSDKMediaPipelinesErrors>(ChimeSDKMediaPipelinesErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MediaPipelineId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-capture-pipelines/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMediaPipelineId());
  return DeleteMediaCapturePipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteMediaCapturePipelineOutcomeCallable ChimeSDKMediaPipelinesClient::DeleteMediaCapturePipelineCallable(const DeleteMediaCapturePipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::DeleteMediaCapturePipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::DeleteMediaCapturePipelineAsync(const DeleteMediaCapturePipelineRequest& request, const DeleteMediaCapturePipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::DeleteMediaCapturePipeline, this, request, handler, context, m_executor.get());
}

DeleteMediaPipelineOutcome ChimeSDKMediaPipelinesClient::DeleteMediaPipeline(const DeleteMediaPipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMediaPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MediaPipelineIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMediaPipeline", "Required field: MediaPipelineId, is not set");
    return DeleteMediaPipelineOutcome(Aws::Client::AWSError<ChimeSDKMediaPipelinesErrors>(ChimeSDKMediaPipelinesErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MediaPipelineId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMediaPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-pipelines/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMediaPipelineId());
  return DeleteMediaPipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteMediaPipelineOutcomeCallable ChimeSDKMediaPipelinesClient::DeleteMediaPipelineCallable(const DeleteMediaPipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::DeleteMediaPipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::DeleteMediaPipelineAsync(const DeleteMediaPipelineRequest& request, const DeleteMediaPipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::DeleteMediaPipeline, this, request, handler, context, m_executor.get());
}

GetMediaCapturePipelineOutcome ChimeSDKMediaPipelinesClient::GetMediaCapturePipeline(const GetMediaCapturePipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MediaPipelineIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMediaCapturePipeline", "Required field: MediaPipelineId, is not set");
    return GetMediaCapturePipelineOutcome(Aws::Client::AWSError<ChimeSDKMediaPipelinesErrors>(ChimeSDKMediaPipelinesErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MediaPipelineId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMediaCapturePipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-capture-pipelines/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMediaPipelineId());
  return GetMediaCapturePipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMediaCapturePipelineOutcomeCallable ChimeSDKMediaPipelinesClient::GetMediaCapturePipelineCallable(const GetMediaCapturePipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::GetMediaCapturePipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::GetMediaCapturePipelineAsync(const GetMediaCapturePipelineRequest& request, const GetMediaCapturePipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::GetMediaCapturePipeline, this, request, handler, context, m_executor.get());
}

GetMediaPipelineOutcome ChimeSDKMediaPipelinesClient::GetMediaPipeline(const GetMediaPipelineRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMediaPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MediaPipelineIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMediaPipeline", "Required field: MediaPipelineId, is not set");
    return GetMediaPipelineOutcome(Aws::Client::AWSError<ChimeSDKMediaPipelinesErrors>(ChimeSDKMediaPipelinesErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MediaPipelineId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMediaPipeline, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-pipelines/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMediaPipelineId());
  return GetMediaPipelineOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMediaPipelineOutcomeCallable ChimeSDKMediaPipelinesClient::GetMediaPipelineCallable(const GetMediaPipelineRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::GetMediaPipeline, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::GetMediaPipelineAsync(const GetMediaPipelineRequest& request, const GetMediaPipelineResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::GetMediaPipeline, this, request, handler, context, m_executor.get());
}

ListMediaCapturePipelinesOutcome ChimeSDKMediaPipelinesClient::ListMediaCapturePipelines(const ListMediaCapturePipelinesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMediaCapturePipelines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMediaCapturePipelines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-capture-pipelines");
  return ListMediaCapturePipelinesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListMediaCapturePipelinesOutcomeCallable ChimeSDKMediaPipelinesClient::ListMediaCapturePipelinesCallable(const ListMediaCapturePipelinesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::ListMediaCapturePipelines, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::ListMediaCapturePipelinesAsync(const ListMediaCapturePipelinesRequest& request, const ListMediaCapturePipelinesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::ListMediaCapturePipelines, this, request, handler, context, m_executor.get());
}

ListMediaPipelinesOutcome ChimeSDKMediaPipelinesClient::ListMediaPipelines(const ListMediaPipelinesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMediaPipelines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMediaPipelines, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sdk-media-pipelines");
  return ListMediaPipelinesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListMediaPipelinesOutcomeCallable ChimeSDKMediaPipelinesClient::ListMediaPipelinesCallable(const ListMediaPipelinesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::ListMediaPipelines, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::ListMediaPipelinesAsync(const ListMediaPipelinesRequest& request, const ListMediaPipelinesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::ListMediaPipelines, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome ChimeSDKMediaPipelinesClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceARNHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceARN, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<ChimeSDKMediaPipelinesErrors>(ChimeSDKMediaPipelinesErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceARN]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable ChimeSDKMediaPipelinesClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::ListTagsForResource, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

TagResourceOutcome ChimeSDKMediaPipelinesClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  ss.str("?operation=tag-resource");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable ChimeSDKMediaPipelinesClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::TagResource, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome ChimeSDKMediaPipelinesClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  ss.str("?operation=untag-resource");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable ChimeSDKMediaPipelinesClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMediaPipelinesClient::UntagResource, this, request, m_executor.get());
}

void ChimeSDKMediaPipelinesClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMediaPipelinesClient::UntagResource, this, request, handler, context, m_executor.get());
}

