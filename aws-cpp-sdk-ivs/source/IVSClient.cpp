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

#include <aws/ivs/IVSClient.h>
#include <aws/ivs/IVSErrorMarshaller.h>
#include <aws/ivs/IVSEndpointProvider.h>
#include <aws/ivs/model/BatchGetChannelRequest.h>
#include <aws/ivs/model/BatchGetStreamKeyRequest.h>
#include <aws/ivs/model/CreateChannelRequest.h>
#include <aws/ivs/model/CreateRecordingConfigurationRequest.h>
#include <aws/ivs/model/CreateStreamKeyRequest.h>
#include <aws/ivs/model/DeleteChannelRequest.h>
#include <aws/ivs/model/DeletePlaybackKeyPairRequest.h>
#include <aws/ivs/model/DeleteRecordingConfigurationRequest.h>
#include <aws/ivs/model/DeleteStreamKeyRequest.h>
#include <aws/ivs/model/GetChannelRequest.h>
#include <aws/ivs/model/GetPlaybackKeyPairRequest.h>
#include <aws/ivs/model/GetRecordingConfigurationRequest.h>
#include <aws/ivs/model/GetStreamRequest.h>
#include <aws/ivs/model/GetStreamKeyRequest.h>
#include <aws/ivs/model/GetStreamSessionRequest.h>
#include <aws/ivs/model/ImportPlaybackKeyPairRequest.h>
#include <aws/ivs/model/ListChannelsRequest.h>
#include <aws/ivs/model/ListPlaybackKeyPairsRequest.h>
#include <aws/ivs/model/ListRecordingConfigurationsRequest.h>
#include <aws/ivs/model/ListStreamKeysRequest.h>
#include <aws/ivs/model/ListStreamSessionsRequest.h>
#include <aws/ivs/model/ListStreamsRequest.h>
#include <aws/ivs/model/ListTagsForResourceRequest.h>
#include <aws/ivs/model/PutMetadataRequest.h>
#include <aws/ivs/model/StopStreamRequest.h>
#include <aws/ivs/model/TagResourceRequest.h>
#include <aws/ivs/model/UntagResourceRequest.h>
#include <aws/ivs/model/UpdateChannelRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::IVS;
using namespace Aws::IVS::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* IVSClient::SERVICE_NAME = "ivs";
const char* IVSClient::ALLOCATION_TAG = "IVSClient";

IVSClient::IVSClient(const IVS::IVSClientConfiguration& clientConfiguration,
                     std::shared_ptr<IVSEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IVSClient::IVSClient(const AWSCredentials& credentials,
                     std::shared_ptr<IVSEndpointProviderBase> endpointProvider,
                     const IVS::IVSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IVSClient::IVSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     std::shared_ptr<IVSEndpointProviderBase> endpointProvider,
                     const IVS::IVSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  IVSClient::IVSClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<IVSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IVSClient::IVSClient(const AWSCredentials& credentials,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IVSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IVSClient::IVSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IVSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IVSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
IVSClient::~IVSClient()
{
}

std::shared_ptr<IVSEndpointProviderBase>& IVSClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void IVSClient::init(const IVS::IVSClientConfiguration& config)
{
  AWSClient::SetServiceClientName("ivs");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void IVSClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchGetChannelOutcome IVSClient::BatchGetChannel(const BatchGetChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/BatchGetChannel");
  return BatchGetChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetChannelOutcomeCallable IVSClient::BatchGetChannelCallable(const BatchGetChannelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchGetChannel, request, m_executor.get());
}

void IVSClient::BatchGetChannelAsync(const BatchGetChannelRequest& request, const BatchGetChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchGetChannel, request, handler, context, m_executor.get());
}

BatchGetStreamKeyOutcome IVSClient::BatchGetStreamKey(const BatchGetStreamKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/BatchGetStreamKey");
  return BatchGetStreamKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetStreamKeyOutcomeCallable IVSClient::BatchGetStreamKeyCallable(const BatchGetStreamKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchGetStreamKey, request, m_executor.get());
}

void IVSClient::BatchGetStreamKeyAsync(const BatchGetStreamKeyRequest& request, const BatchGetStreamKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchGetStreamKey, request, handler, context, m_executor.get());
}

CreateChannelOutcome IVSClient::CreateChannel(const CreateChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateChannel");
  return CreateChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelOutcomeCallable IVSClient::CreateChannelCallable(const CreateChannelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateChannel, request, m_executor.get());
}

void IVSClient::CreateChannelAsync(const CreateChannelRequest& request, const CreateChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateChannel, request, handler, context, m_executor.get());
}

CreateRecordingConfigurationOutcome IVSClient::CreateRecordingConfiguration(const CreateRecordingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateRecordingConfiguration");
  return CreateRecordingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRecordingConfigurationOutcomeCallable IVSClient::CreateRecordingConfigurationCallable(const CreateRecordingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateRecordingConfiguration, request, m_executor.get());
}

void IVSClient::CreateRecordingConfigurationAsync(const CreateRecordingConfigurationRequest& request, const CreateRecordingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateRecordingConfiguration, request, handler, context, m_executor.get());
}

CreateStreamKeyOutcome IVSClient::CreateStreamKey(const CreateStreamKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateStreamKey");
  return CreateStreamKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateStreamKeyOutcomeCallable IVSClient::CreateStreamKeyCallable(const CreateStreamKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateStreamKey, request, m_executor.get());
}

void IVSClient::CreateStreamKeyAsync(const CreateStreamKeyRequest& request, const CreateStreamKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateStreamKey, request, handler, context, m_executor.get());
}

DeleteChannelOutcome IVSClient::DeleteChannel(const DeleteChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteChannel");
  return DeleteChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelOutcomeCallable IVSClient::DeleteChannelCallable(const DeleteChannelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteChannel, request, m_executor.get());
}

void IVSClient::DeleteChannelAsync(const DeleteChannelRequest& request, const DeleteChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteChannel, request, handler, context, m_executor.get());
}

DeletePlaybackKeyPairOutcome IVSClient::DeletePlaybackKeyPair(const DeletePlaybackKeyPairRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeletePlaybackKeyPair");
  return DeletePlaybackKeyPairOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePlaybackKeyPairOutcomeCallable IVSClient::DeletePlaybackKeyPairCallable(const DeletePlaybackKeyPairRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeletePlaybackKeyPair, request, m_executor.get());
}

void IVSClient::DeletePlaybackKeyPairAsync(const DeletePlaybackKeyPairRequest& request, const DeletePlaybackKeyPairResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeletePlaybackKeyPair, request, handler, context, m_executor.get());
}

DeleteRecordingConfigurationOutcome IVSClient::DeleteRecordingConfiguration(const DeleteRecordingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteRecordingConfiguration");
  return DeleteRecordingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRecordingConfigurationOutcomeCallable IVSClient::DeleteRecordingConfigurationCallable(const DeleteRecordingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteRecordingConfiguration, request, m_executor.get());
}

void IVSClient::DeleteRecordingConfigurationAsync(const DeleteRecordingConfigurationRequest& request, const DeleteRecordingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteRecordingConfiguration, request, handler, context, m_executor.get());
}

DeleteStreamKeyOutcome IVSClient::DeleteStreamKey(const DeleteStreamKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteStreamKey");
  return DeleteStreamKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteStreamKeyOutcomeCallable IVSClient::DeleteStreamKeyCallable(const DeleteStreamKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteStreamKey, request, m_executor.get());
}

void IVSClient::DeleteStreamKeyAsync(const DeleteStreamKeyRequest& request, const DeleteStreamKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteStreamKey, request, handler, context, m_executor.get());
}

GetChannelOutcome IVSClient::GetChannel(const GetChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetChannel");
  return GetChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetChannelOutcomeCallable IVSClient::GetChannelCallable(const GetChannelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetChannel, request, m_executor.get());
}

void IVSClient::GetChannelAsync(const GetChannelRequest& request, const GetChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetChannel, request, handler, context, m_executor.get());
}

GetPlaybackKeyPairOutcome IVSClient::GetPlaybackKeyPair(const GetPlaybackKeyPairRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetPlaybackKeyPair");
  return GetPlaybackKeyPairOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetPlaybackKeyPairOutcomeCallable IVSClient::GetPlaybackKeyPairCallable(const GetPlaybackKeyPairRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetPlaybackKeyPair, request, m_executor.get());
}

void IVSClient::GetPlaybackKeyPairAsync(const GetPlaybackKeyPairRequest& request, const GetPlaybackKeyPairResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetPlaybackKeyPair, request, handler, context, m_executor.get());
}

GetRecordingConfigurationOutcome IVSClient::GetRecordingConfiguration(const GetRecordingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRecordingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetRecordingConfiguration");
  return GetRecordingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRecordingConfigurationOutcomeCallable IVSClient::GetRecordingConfigurationCallable(const GetRecordingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRecordingConfiguration, request, m_executor.get());
}

void IVSClient::GetRecordingConfigurationAsync(const GetRecordingConfigurationRequest& request, const GetRecordingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRecordingConfiguration, request, handler, context, m_executor.get());
}

GetStreamOutcome IVSClient::GetStream(const GetStreamRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetStream");
  return GetStreamOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetStreamOutcomeCallable IVSClient::GetStreamCallable(const GetStreamRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetStream, request, m_executor.get());
}

void IVSClient::GetStreamAsync(const GetStreamRequest& request, const GetStreamResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetStream, request, handler, context, m_executor.get());
}

GetStreamKeyOutcome IVSClient::GetStreamKey(const GetStreamKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStreamKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetStreamKey");
  return GetStreamKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetStreamKeyOutcomeCallable IVSClient::GetStreamKeyCallable(const GetStreamKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetStreamKey, request, m_executor.get());
}

void IVSClient::GetStreamKeyAsync(const GetStreamKeyRequest& request, const GetStreamKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetStreamKey, request, handler, context, m_executor.get());
}

GetStreamSessionOutcome IVSClient::GetStreamSession(const GetStreamSessionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetStreamSession, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetStreamSession, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetStreamSession");
  return GetStreamSessionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetStreamSessionOutcomeCallable IVSClient::GetStreamSessionCallable(const GetStreamSessionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetStreamSession, request, m_executor.get());
}

void IVSClient::GetStreamSessionAsync(const GetStreamSessionRequest& request, const GetStreamSessionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetStreamSession, request, handler, context, m_executor.get());
}

ImportPlaybackKeyPairOutcome IVSClient::ImportPlaybackKeyPair(const ImportPlaybackKeyPairRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportPlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportPlaybackKeyPair, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ImportPlaybackKeyPair");
  return ImportPlaybackKeyPairOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportPlaybackKeyPairOutcomeCallable IVSClient::ImportPlaybackKeyPairCallable(const ImportPlaybackKeyPairRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ImportPlaybackKeyPair, request, m_executor.get());
}

void IVSClient::ImportPlaybackKeyPairAsync(const ImportPlaybackKeyPairRequest& request, const ImportPlaybackKeyPairResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ImportPlaybackKeyPair, request, handler, context, m_executor.get());
}

ListChannelsOutcome IVSClient::ListChannels(const ListChannelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListChannels");
  return ListChannelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListChannelsOutcomeCallable IVSClient::ListChannelsCallable(const ListChannelsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListChannels, request, m_executor.get());
}

void IVSClient::ListChannelsAsync(const ListChannelsRequest& request, const ListChannelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListChannels, request, handler, context, m_executor.get());
}

ListPlaybackKeyPairsOutcome IVSClient::ListPlaybackKeyPairs(const ListPlaybackKeyPairsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPlaybackKeyPairs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPlaybackKeyPairs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListPlaybackKeyPairs");
  return ListPlaybackKeyPairsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPlaybackKeyPairsOutcomeCallable IVSClient::ListPlaybackKeyPairsCallable(const ListPlaybackKeyPairsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListPlaybackKeyPairs, request, m_executor.get());
}

void IVSClient::ListPlaybackKeyPairsAsync(const ListPlaybackKeyPairsRequest& request, const ListPlaybackKeyPairsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListPlaybackKeyPairs, request, handler, context, m_executor.get());
}

ListRecordingConfigurationsOutcome IVSClient::ListRecordingConfigurations(const ListRecordingConfigurationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRecordingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRecordingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListRecordingConfigurations");
  return ListRecordingConfigurationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRecordingConfigurationsOutcomeCallable IVSClient::ListRecordingConfigurationsCallable(const ListRecordingConfigurationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListRecordingConfigurations, request, m_executor.get());
}

void IVSClient::ListRecordingConfigurationsAsync(const ListRecordingConfigurationsRequest& request, const ListRecordingConfigurationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListRecordingConfigurations, request, handler, context, m_executor.get());
}

ListStreamKeysOutcome IVSClient::ListStreamKeys(const ListStreamKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListStreamKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListStreamKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListStreamKeys");
  return ListStreamKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListStreamKeysOutcomeCallable IVSClient::ListStreamKeysCallable(const ListStreamKeysRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListStreamKeys, request, m_executor.get());
}

void IVSClient::ListStreamKeysAsync(const ListStreamKeysRequest& request, const ListStreamKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListStreamKeys, request, handler, context, m_executor.get());
}

ListStreamSessionsOutcome IVSClient::ListStreamSessions(const ListStreamSessionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListStreamSessions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListStreamSessions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListStreamSessions");
  return ListStreamSessionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListStreamSessionsOutcomeCallable IVSClient::ListStreamSessionsCallable(const ListStreamSessionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListStreamSessions, request, m_executor.get());
}

void IVSClient::ListStreamSessionsAsync(const ListStreamSessionsRequest& request, const ListStreamSessionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListStreamSessions, request, handler, context, m_executor.get());
}

ListStreamsOutcome IVSClient::ListStreams(const ListStreamsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListStreams, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListStreams, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListStreams");
  return ListStreamsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListStreamsOutcomeCallable IVSClient::ListStreamsCallable(const ListStreamsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListStreams, request, m_executor.get());
}

void IVSClient::ListStreamsAsync(const ListStreamsRequest& request, const ListStreamsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListStreams, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome IVSClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<IVSErrors>(IVSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable IVSClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void IVSClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

PutMetadataOutcome IVSClient::PutMetadata(const PutMetadataRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/PutMetadata");
  return PutMetadataOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutMetadataOutcomeCallable IVSClient::PutMetadataCallable(const PutMetadataRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutMetadata, request, m_executor.get());
}

void IVSClient::PutMetadataAsync(const PutMetadataRequest& request, const PutMetadataResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutMetadata, request, handler, context, m_executor.get());
}

StopStreamOutcome IVSClient::StopStream(const StopStreamRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopStream, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/StopStream");
  return StopStreamOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopStreamOutcomeCallable IVSClient::StopStreamCallable(const StopStreamRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StopStream, request, m_executor.get());
}

void IVSClient::StopStreamAsync(const StopStreamRequest& request, const StopStreamResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StopStream, request, handler, context, m_executor.get());
}

TagResourceOutcome IVSClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<IVSErrors>(IVSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable IVSClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void IVSClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome IVSClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IVSErrors>(IVSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IVSErrors>(IVSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable IVSClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void IVSClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateChannelOutcome IVSClient::UpdateChannel(const UpdateChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/UpdateChannel");
  return UpdateChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateChannelOutcomeCallable IVSClient::UpdateChannelCallable(const UpdateChannelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateChannel, request, m_executor.get());
}

void IVSClient::UpdateChannelAsync(const UpdateChannelRequest& request, const UpdateChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateChannel, request, handler, context, m_executor.get());
}

