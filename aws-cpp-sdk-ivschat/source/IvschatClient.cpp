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

#include <aws/ivschat/IvschatClient.h>
#include <aws/ivschat/IvschatErrorMarshaller.h>
#include <aws/ivschat/IvschatEndpointProvider.h>
#include <aws/ivschat/model/CreateChatTokenRequest.h>
#include <aws/ivschat/model/CreateLoggingConfigurationRequest.h>
#include <aws/ivschat/model/CreateRoomRequest.h>
#include <aws/ivschat/model/DeleteLoggingConfigurationRequest.h>
#include <aws/ivschat/model/DeleteMessageRequest.h>
#include <aws/ivschat/model/DeleteRoomRequest.h>
#include <aws/ivschat/model/DisconnectUserRequest.h>
#include <aws/ivschat/model/GetLoggingConfigurationRequest.h>
#include <aws/ivschat/model/GetRoomRequest.h>
#include <aws/ivschat/model/ListLoggingConfigurationsRequest.h>
#include <aws/ivschat/model/ListRoomsRequest.h>
#include <aws/ivschat/model/ListTagsForResourceRequest.h>
#include <aws/ivschat/model/SendEventRequest.h>
#include <aws/ivschat/model/TagResourceRequest.h>
#include <aws/ivschat/model/UntagResourceRequest.h>
#include <aws/ivschat/model/UpdateLoggingConfigurationRequest.h>
#include <aws/ivschat/model/UpdateRoomRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ivschat;
using namespace Aws::ivschat::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* IvschatClient::SERVICE_NAME = "ivschat";
const char* IvschatClient::ALLOCATION_TAG = "IvschatClient";

IvschatClient::IvschatClient(const ivschat::IvschatClientConfiguration& clientConfiguration,
                             std::shared_ptr<IvschatEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IvschatClient::IvschatClient(const AWSCredentials& credentials,
                             std::shared_ptr<IvschatEndpointProviderBase> endpointProvider,
                             const ivschat::IvschatClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IvschatClient::IvschatClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             std::shared_ptr<IvschatEndpointProviderBase> endpointProvider,
                             const ivschat::IvschatClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  IvschatClient::IvschatClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<IvschatEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IvschatClient::IvschatClient(const AWSCredentials& credentials,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IvschatEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IvschatClient::IvschatClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IvschatErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IvschatEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
IvschatClient::~IvschatClient()
{
}

std::shared_ptr<IvschatEndpointProviderBase>& IvschatClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void IvschatClient::init(const ivschat::IvschatClientConfiguration& config)
{
  AWSClient::SetServiceClientName("ivschat");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void IvschatClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateChatTokenOutcome IvschatClient::CreateChatToken(const CreateChatTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChatToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChatToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateChatToken");
  return CreateChatTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChatTokenOutcomeCallable IvschatClient::CreateChatTokenCallable(const CreateChatTokenRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateChatToken, request, m_executor.get());
}

void IvschatClient::CreateChatTokenAsync(const CreateChatTokenRequest& request, const CreateChatTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateChatToken, request, handler, context, m_executor.get());
}

CreateLoggingConfigurationOutcome IvschatClient::CreateLoggingConfiguration(const CreateLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateLoggingConfiguration");
  return CreateLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateLoggingConfigurationOutcomeCallable IvschatClient::CreateLoggingConfigurationCallable(const CreateLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateLoggingConfiguration, request, m_executor.get());
}

void IvschatClient::CreateLoggingConfigurationAsync(const CreateLoggingConfigurationRequest& request, const CreateLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateLoggingConfiguration, request, handler, context, m_executor.get());
}

CreateRoomOutcome IvschatClient::CreateRoom(const CreateRoomRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/CreateRoom");
  return CreateRoomOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRoomOutcomeCallable IvschatClient::CreateRoomCallable(const CreateRoomRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateRoom, request, m_executor.get());
}

void IvschatClient::CreateRoomAsync(const CreateRoomRequest& request, const CreateRoomResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateRoom, request, handler, context, m_executor.get());
}

DeleteLoggingConfigurationOutcome IvschatClient::DeleteLoggingConfiguration(const DeleteLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteLoggingConfiguration");
  return DeleteLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteLoggingConfigurationOutcomeCallable IvschatClient::DeleteLoggingConfigurationCallable(const DeleteLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteLoggingConfiguration, request, m_executor.get());
}

void IvschatClient::DeleteLoggingConfigurationAsync(const DeleteLoggingConfigurationRequest& request, const DeleteLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteLoggingConfiguration, request, handler, context, m_executor.get());
}

DeleteMessageOutcome IvschatClient::DeleteMessage(const DeleteMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteMessage");
  return DeleteMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteMessageOutcomeCallable IvschatClient::DeleteMessageCallable(const DeleteMessageRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteMessage, request, m_executor.get());
}

void IvschatClient::DeleteMessageAsync(const DeleteMessageRequest& request, const DeleteMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteMessage, request, handler, context, m_executor.get());
}

DeleteRoomOutcome IvschatClient::DeleteRoom(const DeleteRoomRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DeleteRoom");
  return DeleteRoomOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRoomOutcomeCallable IvschatClient::DeleteRoomCallable(const DeleteRoomRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteRoom, request, m_executor.get());
}

void IvschatClient::DeleteRoomAsync(const DeleteRoomRequest& request, const DeleteRoomResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteRoom, request, handler, context, m_executor.get());
}

DisconnectUserOutcome IvschatClient::DisconnectUser(const DisconnectUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisconnectUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisconnectUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/DisconnectUser");
  return DisconnectUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisconnectUserOutcomeCallable IvschatClient::DisconnectUserCallable(const DisconnectUserRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisconnectUser, request, m_executor.get());
}

void IvschatClient::DisconnectUserAsync(const DisconnectUserRequest& request, const DisconnectUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisconnectUser, request, handler, context, m_executor.get());
}

GetLoggingConfigurationOutcome IvschatClient::GetLoggingConfiguration(const GetLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetLoggingConfiguration");
  return GetLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetLoggingConfigurationOutcomeCallable IvschatClient::GetLoggingConfigurationCallable(const GetLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetLoggingConfiguration, request, m_executor.get());
}

void IvschatClient::GetLoggingConfigurationAsync(const GetLoggingConfigurationRequest& request, const GetLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetLoggingConfiguration, request, handler, context, m_executor.get());
}

GetRoomOutcome IvschatClient::GetRoom(const GetRoomRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/GetRoom");
  return GetRoomOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRoomOutcomeCallable IvschatClient::GetRoomCallable(const GetRoomRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRoom, request, m_executor.get());
}

void IvschatClient::GetRoomAsync(const GetRoomRequest& request, const GetRoomResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRoom, request, handler, context, m_executor.get());
}

ListLoggingConfigurationsOutcome IvschatClient::ListLoggingConfigurations(const ListLoggingConfigurationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListLoggingConfigurations");
  return ListLoggingConfigurationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListLoggingConfigurationsOutcomeCallable IvschatClient::ListLoggingConfigurationsCallable(const ListLoggingConfigurationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListLoggingConfigurations, request, m_executor.get());
}

void IvschatClient::ListLoggingConfigurationsAsync(const ListLoggingConfigurationsRequest& request, const ListLoggingConfigurationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListLoggingConfigurations, request, handler, context, m_executor.get());
}

ListRoomsOutcome IvschatClient::ListRooms(const ListRoomsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRooms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRooms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ListRooms");
  return ListRoomsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRoomsOutcomeCallable IvschatClient::ListRoomsCallable(const ListRoomsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListRooms, request, m_executor.get());
}

void IvschatClient::ListRoomsAsync(const ListRoomsRequest& request, const ListRoomsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListRooms, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome IvschatClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<IvschatErrors>(IvschatErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable IvschatClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void IvschatClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

SendEventOutcome IvschatClient::SendEvent(const SendEventRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SendEvent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SendEvent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/SendEvent");
  return SendEventOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SendEventOutcomeCallable IvschatClient::SendEventCallable(const SendEventRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(SendEvent, request, m_executor.get());
}

void IvschatClient::SendEventAsync(const SendEventRequest& request, const SendEventResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(SendEvent, request, handler, context, m_executor.get());
}

TagResourceOutcome IvschatClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<IvschatErrors>(IvschatErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable IvschatClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void IvschatClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome IvschatClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IvschatErrors>(IvschatErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IvschatErrors>(IvschatErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable IvschatClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void IvschatClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateLoggingConfigurationOutcome IvschatClient::UpdateLoggingConfiguration(const UpdateLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/UpdateLoggingConfiguration");
  return UpdateLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateLoggingConfigurationOutcomeCallable IvschatClient::UpdateLoggingConfigurationCallable(const UpdateLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateLoggingConfiguration, request, m_executor.get());
}

void IvschatClient::UpdateLoggingConfigurationAsync(const UpdateLoggingConfigurationRequest& request, const UpdateLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateLoggingConfiguration, request, handler, context, m_executor.get());
}

UpdateRoomOutcome IvschatClient::UpdateRoom(const UpdateRoomRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRoom, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/UpdateRoom");
  return UpdateRoomOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRoomOutcomeCallable IvschatClient::UpdateRoomCallable(const UpdateRoomRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateRoom, request, m_executor.get());
}

void IvschatClient::UpdateRoomAsync(const UpdateRoomRequest& request, const UpdateRoomResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateRoom, request, handler, context, m_executor.get());
}

