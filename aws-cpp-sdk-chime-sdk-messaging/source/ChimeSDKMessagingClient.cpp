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

#include <aws/chime-sdk-messaging/ChimeSDKMessagingClient.h>
#include <aws/chime-sdk-messaging/ChimeSDKMessagingErrorMarshaller.h>
#include <aws/chime-sdk-messaging/ChimeSDKMessagingEndpointProvider.h>
#include <aws/chime-sdk-messaging/model/AssociateChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/BatchCreateChannelMembershipRequest.h>
#include <aws/chime-sdk-messaging/model/ChannelFlowCallbackRequest.h>
#include <aws/chime-sdk-messaging/model/CreateChannelRequest.h>
#include <aws/chime-sdk-messaging/model/CreateChannelBanRequest.h>
#include <aws/chime-sdk-messaging/model/CreateChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/CreateChannelMembershipRequest.h>
#include <aws/chime-sdk-messaging/model/CreateChannelModeratorRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelBanRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelMembershipRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelMessageRequest.h>
#include <aws/chime-sdk-messaging/model/DeleteChannelModeratorRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelBanRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelMembershipRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelMembershipForAppInstanceUserRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelModeratedByAppInstanceUserRequest.h>
#include <aws/chime-sdk-messaging/model/DescribeChannelModeratorRequest.h>
#include <aws/chime-sdk-messaging/model/DisassociateChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/GetChannelMembershipPreferencesRequest.h>
#include <aws/chime-sdk-messaging/model/GetChannelMessageRequest.h>
#include <aws/chime-sdk-messaging/model/GetChannelMessageStatusRequest.h>
#include <aws/chime-sdk-messaging/model/GetMessagingSessionEndpointRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelBansRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelFlowsRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelMembershipsRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelMembershipsForAppInstanceUserRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelMessagesRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelModeratorsRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelsRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelsAssociatedWithChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/ListChannelsModeratedByAppInstanceUserRequest.h>
#include <aws/chime-sdk-messaging/model/ListSubChannelsRequest.h>
#include <aws/chime-sdk-messaging/model/ListTagsForResourceRequest.h>
#include <aws/chime-sdk-messaging/model/PutChannelMembershipPreferencesRequest.h>
#include <aws/chime-sdk-messaging/model/RedactChannelMessageRequest.h>
#include <aws/chime-sdk-messaging/model/SearchChannelsRequest.h>
#include <aws/chime-sdk-messaging/model/SendChannelMessageRequest.h>
#include <aws/chime-sdk-messaging/model/TagResourceRequest.h>
#include <aws/chime-sdk-messaging/model/UntagResourceRequest.h>
#include <aws/chime-sdk-messaging/model/UpdateChannelRequest.h>
#include <aws/chime-sdk-messaging/model/UpdateChannelFlowRequest.h>
#include <aws/chime-sdk-messaging/model/UpdateChannelMessageRequest.h>
#include <aws/chime-sdk-messaging/model/UpdateChannelReadMarkerRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ChimeSDKMessaging;
using namespace Aws::ChimeSDKMessaging::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ChimeSDKMessagingClient::SERVICE_NAME = "chime";
const char* ChimeSDKMessagingClient::ALLOCATION_TAG = "ChimeSDKMessagingClient";

ChimeSDKMessagingClient::ChimeSDKMessagingClient(const ChimeSDKMessaging::ChimeSDKMessagingClientConfiguration& clientConfiguration,
                                                 std::shared_ptr<ChimeSDKMessagingEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMessagingClient::ChimeSDKMessagingClient(const AWSCredentials& credentials,
                                                 std::shared_ptr<ChimeSDKMessagingEndpointProviderBase> endpointProvider,
                                                 const ChimeSDKMessaging::ChimeSDKMessagingClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMessagingClient::ChimeSDKMessagingClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                 std::shared_ptr<ChimeSDKMessagingEndpointProviderBase> endpointProvider,
                                                 const ChimeSDKMessaging::ChimeSDKMessagingClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ChimeSDKMessagingClient::ChimeSDKMessagingClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ChimeSDKMessagingEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMessagingClient::ChimeSDKMessagingClient(const AWSCredentials& credentials,
                                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMessagingEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMessagingClient::ChimeSDKMessagingClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMessagingErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMessagingEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ChimeSDKMessagingClient::~ChimeSDKMessagingClient()
{
}

std::shared_ptr<ChimeSDKMessagingEndpointProviderBase>& ChimeSDKMessagingClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ChimeSDKMessagingClient::init(const ChimeSDKMessaging::ChimeSDKMessagingClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Chime SDK Messaging");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ChimeSDKMessagingClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateChannelFlowOutcome ChimeSDKMessagingClient::AssociateChannelFlow(const AssociateChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateChannelFlow", "Required field: ChannelArn, is not set");
    return AssociateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateChannelFlow", "Required field: ChimeBearer, is not set");
    return AssociateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flow");
  return AssociateChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AssociateChannelFlowOutcomeCallable ChimeSDKMessagingClient::AssociateChannelFlowCallable(const AssociateChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::AssociateChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::AssociateChannelFlowAsync(const AssociateChannelFlowRequest& request, const AssociateChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::AssociateChannelFlow, this, request, handler, context, m_executor.get());
}

BatchCreateChannelMembershipOutcome ChimeSDKMessagingClient::BatchCreateChannelMembership(const BatchCreateChannelMembershipRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchCreateChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchCreateChannelMembership", "Required field: ChannelArn, is not set");
    return BatchCreateChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchCreateChannelMembership", "Required field: ChimeBearer, is not set");
    return BatchCreateChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchCreateChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships");
  ss.str("?operation=batch-create");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return BatchCreateChannelMembershipOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchCreateChannelMembershipOutcomeCallable ChimeSDKMessagingClient::BatchCreateChannelMembershipCallable(const BatchCreateChannelMembershipRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::BatchCreateChannelMembership, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::BatchCreateChannelMembershipAsync(const BatchCreateChannelMembershipRequest& request, const BatchCreateChannelMembershipResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::BatchCreateChannelMembership, this, request, handler, context, m_executor.get());
}

ChannelFlowCallbackOutcome ChimeSDKMessagingClient::ChannelFlowCallback(const ChannelFlowCallbackRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ChannelFlowCallback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ChannelFlowCallback", "Required field: ChannelArn, is not set");
    return ChannelFlowCallbackOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ChannelFlowCallback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  ss.str("?operation=channel-flow-callback");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ChannelFlowCallbackOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ChannelFlowCallbackOutcomeCallable ChimeSDKMessagingClient::ChannelFlowCallbackCallable(const ChannelFlowCallbackRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ChannelFlowCallback, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ChannelFlowCallbackAsync(const ChannelFlowCallbackRequest& request, const ChannelFlowCallbackResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ChannelFlowCallback, this, request, handler, context, m_executor.get());
}

CreateChannelOutcome ChimeSDKMessagingClient::CreateChannel(const CreateChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannel", "Required field: ChimeBearer, is not set");
    return CreateChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  return CreateChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelOutcomeCallable ChimeSDKMessagingClient::CreateChannelCallable(const CreateChannelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::CreateChannel, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::CreateChannelAsync(const CreateChannelRequest& request, const CreateChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::CreateChannel, this, request, handler, context, m_executor.get());
}

CreateChannelBanOutcome ChimeSDKMessagingClient::CreateChannelBan(const CreateChannelBanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelBan", "Required field: ChannelArn, is not set");
    return CreateChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelBan", "Required field: ChimeBearer, is not set");
    return CreateChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bans");
  return CreateChannelBanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelBanOutcomeCallable ChimeSDKMessagingClient::CreateChannelBanCallable(const CreateChannelBanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::CreateChannelBan, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::CreateChannelBanAsync(const CreateChannelBanRequest& request, const CreateChannelBanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::CreateChannelBan, this, request, handler, context, m_executor.get());
}

CreateChannelFlowOutcome ChimeSDKMessagingClient::CreateChannelFlow(const CreateChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flows");
  return CreateChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelFlowOutcomeCallable ChimeSDKMessagingClient::CreateChannelFlowCallable(const CreateChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::CreateChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::CreateChannelFlowAsync(const CreateChannelFlowRequest& request, const CreateChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::CreateChannelFlow, this, request, handler, context, m_executor.get());
}

CreateChannelMembershipOutcome ChimeSDKMessagingClient::CreateChannelMembership(const CreateChannelMembershipRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelMembership", "Required field: ChannelArn, is not set");
    return CreateChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelMembership", "Required field: ChimeBearer, is not set");
    return CreateChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships");
  return CreateChannelMembershipOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelMembershipOutcomeCallable ChimeSDKMessagingClient::CreateChannelMembershipCallable(const CreateChannelMembershipRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::CreateChannelMembership, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::CreateChannelMembershipAsync(const CreateChannelMembershipRequest& request, const CreateChannelMembershipResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::CreateChannelMembership, this, request, handler, context, m_executor.get());
}

CreateChannelModeratorOutcome ChimeSDKMessagingClient::CreateChannelModerator(const CreateChannelModeratorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelModerator", "Required field: ChannelArn, is not set");
    return CreateChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateChannelModerator", "Required field: ChimeBearer, is not set");
    return CreateChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/moderators");
  return CreateChannelModeratorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateChannelModeratorOutcomeCallable ChimeSDKMessagingClient::CreateChannelModeratorCallable(const CreateChannelModeratorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::CreateChannelModerator, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::CreateChannelModeratorAsync(const CreateChannelModeratorRequest& request, const CreateChannelModeratorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::CreateChannelModerator, this, request, handler, context, m_executor.get());
}

DeleteChannelOutcome ChimeSDKMessagingClient::DeleteChannel(const DeleteChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannel", "Required field: ChannelArn, is not set");
    return DeleteChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannel", "Required field: ChimeBearer, is not set");
    return DeleteChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  return DeleteChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelOutcomeCallable ChimeSDKMessagingClient::DeleteChannelCallable(const DeleteChannelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannel, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelAsync(const DeleteChannelRequest& request, const DeleteChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannel, this, request, handler, context, m_executor.get());
}

DeleteChannelBanOutcome ChimeSDKMessagingClient::DeleteChannelBan(const DeleteChannelBanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelBan", "Required field: ChannelArn, is not set");
    return DeleteChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelBan", "Required field: MemberArn, is not set");
    return DeleteChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelBan", "Required field: ChimeBearer, is not set");
    return DeleteChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  return DeleteChannelBanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelBanOutcomeCallable ChimeSDKMessagingClient::DeleteChannelBanCallable(const DeleteChannelBanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannelBan, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelBanAsync(const DeleteChannelBanRequest& request, const DeleteChannelBanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannelBan, this, request, handler, context, m_executor.get());
}

DeleteChannelFlowOutcome ChimeSDKMessagingClient::DeleteChannelFlow(const DeleteChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelFlowArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelFlow", "Required field: ChannelFlowArn, is not set");
    return DeleteChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelFlowArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flows/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelFlowArn());
  return DeleteChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelFlowOutcomeCallable ChimeSDKMessagingClient::DeleteChannelFlowCallable(const DeleteChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelFlowAsync(const DeleteChannelFlowRequest& request, const DeleteChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannelFlow, this, request, handler, context, m_executor.get());
}

DeleteChannelMembershipOutcome ChimeSDKMessagingClient::DeleteChannelMembership(const DeleteChannelMembershipRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMembership", "Required field: ChannelArn, is not set");
    return DeleteChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMembership", "Required field: MemberArn, is not set");
    return DeleteChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMembership", "Required field: ChimeBearer, is not set");
    return DeleteChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  return DeleteChannelMembershipOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelMembershipOutcomeCallable ChimeSDKMessagingClient::DeleteChannelMembershipCallable(const DeleteChannelMembershipRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannelMembership, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelMembershipAsync(const DeleteChannelMembershipRequest& request, const DeleteChannelMembershipResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannelMembership, this, request, handler, context, m_executor.get());
}

DeleteChannelMessageOutcome ChimeSDKMessagingClient::DeleteChannelMessage(const DeleteChannelMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMessage", "Required field: ChannelArn, is not set");
    return DeleteChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MessageIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMessage", "Required field: MessageId, is not set");
    return DeleteChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MessageId]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelMessage", "Required field: ChimeBearer, is not set");
    return DeleteChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMessageId());
  return DeleteChannelMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelMessageOutcomeCallable ChimeSDKMessagingClient::DeleteChannelMessageCallable(const DeleteChannelMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannelMessage, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelMessageAsync(const DeleteChannelMessageRequest& request, const DeleteChannelMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannelMessage, this, request, handler, context, m_executor.get());
}

DeleteChannelModeratorOutcome ChimeSDKMessagingClient::DeleteChannelModerator(const DeleteChannelModeratorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelModerator", "Required field: ChannelArn, is not set");
    return DeleteChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChannelModeratorArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelModerator", "Required field: ChannelModeratorArn, is not set");
    return DeleteChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelModeratorArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteChannelModerator", "Required field: ChimeBearer, is not set");
    return DeleteChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/moderators/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelModeratorArn());
  return DeleteChannelModeratorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteChannelModeratorOutcomeCallable ChimeSDKMessagingClient::DeleteChannelModeratorCallable(const DeleteChannelModeratorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DeleteChannelModerator, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DeleteChannelModeratorAsync(const DeleteChannelModeratorRequest& request, const DeleteChannelModeratorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DeleteChannelModerator, this, request, handler, context, m_executor.get());
}

DescribeChannelOutcome ChimeSDKMessagingClient::DescribeChannel(const DescribeChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannel", "Required field: ChannelArn, is not set");
    return DescribeChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannel", "Required field: ChimeBearer, is not set");
    return DescribeChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  return DescribeChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelOutcomeCallable ChimeSDKMessagingClient::DescribeChannelCallable(const DescribeChannelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannel, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelAsync(const DescribeChannelRequest& request, const DescribeChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannel, this, request, handler, context, m_executor.get());
}

DescribeChannelBanOutcome ChimeSDKMessagingClient::DescribeChannelBan(const DescribeChannelBanRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelBan", "Required field: ChannelArn, is not set");
    return DescribeChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelBan", "Required field: MemberArn, is not set");
    return DescribeChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelBan", "Required field: ChimeBearer, is not set");
    return DescribeChannelBanOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelBan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bans/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  return DescribeChannelBanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelBanOutcomeCallable ChimeSDKMessagingClient::DescribeChannelBanCallable(const DescribeChannelBanRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelBan, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelBanAsync(const DescribeChannelBanRequest& request, const DescribeChannelBanResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelBan, this, request, handler, context, m_executor.get());
}

DescribeChannelFlowOutcome ChimeSDKMessagingClient::DescribeChannelFlow(const DescribeChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelFlowArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelFlow", "Required field: ChannelFlowArn, is not set");
    return DescribeChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelFlowArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flows/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelFlowArn());
  return DescribeChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelFlowOutcomeCallable ChimeSDKMessagingClient::DescribeChannelFlowCallable(const DescribeChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelFlowAsync(const DescribeChannelFlowRequest& request, const DescribeChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelFlow, this, request, handler, context, m_executor.get());
}

DescribeChannelMembershipOutcome ChimeSDKMessagingClient::DescribeChannelMembership(const DescribeChannelMembershipRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembership", "Required field: ChannelArn, is not set");
    return DescribeChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembership", "Required field: MemberArn, is not set");
    return DescribeChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembership", "Required field: ChimeBearer, is not set");
    return DescribeChannelMembershipOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  return DescribeChannelMembershipOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelMembershipOutcomeCallable ChimeSDKMessagingClient::DescribeChannelMembershipCallable(const DescribeChannelMembershipRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelMembership, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelMembershipAsync(const DescribeChannelMembershipRequest& request, const DescribeChannelMembershipResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelMembership, this, request, handler, context, m_executor.get());
}

DescribeChannelMembershipForAppInstanceUserOutcome ChimeSDKMessagingClient::DescribeChannelMembershipForAppInstanceUser(const DescribeChannelMembershipForAppInstanceUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelMembershipForAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembershipForAppInstanceUser", "Required field: ChannelArn, is not set");
    return DescribeChannelMembershipForAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.AppInstanceUserArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembershipForAppInstanceUser", "Required field: AppInstanceUserArn, is not set");
    return DescribeChannelMembershipForAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppInstanceUserArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelMembershipForAppInstanceUser", "Required field: ChimeBearer, is not set");
    return DescribeChannelMembershipForAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelMembershipForAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  ss.str("?scope=app-instance-user-membership");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return DescribeChannelMembershipForAppInstanceUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelMembershipForAppInstanceUserOutcomeCallable ChimeSDKMessagingClient::DescribeChannelMembershipForAppInstanceUserCallable(const DescribeChannelMembershipForAppInstanceUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelMembershipForAppInstanceUser, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelMembershipForAppInstanceUserAsync(const DescribeChannelMembershipForAppInstanceUserRequest& request, const DescribeChannelMembershipForAppInstanceUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelMembershipForAppInstanceUser, this, request, handler, context, m_executor.get());
}

DescribeChannelModeratedByAppInstanceUserOutcome ChimeSDKMessagingClient::DescribeChannelModeratedByAppInstanceUser(const DescribeChannelModeratedByAppInstanceUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelModeratedByAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModeratedByAppInstanceUser", "Required field: ChannelArn, is not set");
    return DescribeChannelModeratedByAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.AppInstanceUserArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModeratedByAppInstanceUser", "Required field: AppInstanceUserArn, is not set");
    return DescribeChannelModeratedByAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppInstanceUserArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModeratedByAppInstanceUser", "Required field: ChimeBearer, is not set");
    return DescribeChannelModeratedByAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelModeratedByAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  ss.str("?scope=app-instance-user-moderated-channel");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return DescribeChannelModeratedByAppInstanceUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelModeratedByAppInstanceUserOutcomeCallable ChimeSDKMessagingClient::DescribeChannelModeratedByAppInstanceUserCallable(const DescribeChannelModeratedByAppInstanceUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelModeratedByAppInstanceUser, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelModeratedByAppInstanceUserAsync(const DescribeChannelModeratedByAppInstanceUserRequest& request, const DescribeChannelModeratedByAppInstanceUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelModeratedByAppInstanceUser, this, request, handler, context, m_executor.get());
}

DescribeChannelModeratorOutcome ChimeSDKMessagingClient::DescribeChannelModerator(const DescribeChannelModeratorRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModerator", "Required field: ChannelArn, is not set");
    return DescribeChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChannelModeratorArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModerator", "Required field: ChannelModeratorArn, is not set");
    return DescribeChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelModeratorArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeChannelModerator", "Required field: ChimeBearer, is not set");
    return DescribeChannelModeratorOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeChannelModerator, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/moderators/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelModeratorArn());
  return DescribeChannelModeratorOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeChannelModeratorOutcomeCallable ChimeSDKMessagingClient::DescribeChannelModeratorCallable(const DescribeChannelModeratorRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DescribeChannelModerator, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DescribeChannelModeratorAsync(const DescribeChannelModeratorRequest& request, const DescribeChannelModeratorResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DescribeChannelModerator, this, request, handler, context, m_executor.get());
}

DisassociateChannelFlowOutcome ChimeSDKMessagingClient::DisassociateChannelFlow(const DisassociateChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateChannelFlow", "Required field: ChannelArn, is not set");
    return DisassociateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChannelFlowArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateChannelFlow", "Required field: ChannelFlowArn, is not set");
    return DisassociateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelFlowArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateChannelFlow", "Required field: ChimeBearer, is not set");
    return DisassociateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flow/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelFlowArn());
  return DisassociateChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateChannelFlowOutcomeCallable ChimeSDKMessagingClient::DisassociateChannelFlowCallable(const DisassociateChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::DisassociateChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::DisassociateChannelFlowAsync(const DisassociateChannelFlowRequest& request, const DisassociateChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::DisassociateChannelFlow, this, request, handler, context, m_executor.get());
}

GetChannelMembershipPreferencesOutcome ChimeSDKMessagingClient::GetChannelMembershipPreferences(const GetChannelMembershipPreferencesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChannelMembershipPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMembershipPreferences", "Required field: ChannelArn, is not set");
    return GetChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMembershipPreferences", "Required field: MemberArn, is not set");
    return GetChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMembershipPreferences", "Required field: ChimeBearer, is not set");
    return GetChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChannelMembershipPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/preferences");
  return GetChannelMembershipPreferencesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetChannelMembershipPreferencesOutcomeCallable ChimeSDKMessagingClient::GetChannelMembershipPreferencesCallable(const GetChannelMembershipPreferencesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::GetChannelMembershipPreferences, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::GetChannelMembershipPreferencesAsync(const GetChannelMembershipPreferencesRequest& request, const GetChannelMembershipPreferencesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::GetChannelMembershipPreferences, this, request, handler, context, m_executor.get());
}

GetChannelMessageOutcome ChimeSDKMessagingClient::GetChannelMessage(const GetChannelMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessage", "Required field: ChannelArn, is not set");
    return GetChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MessageIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessage", "Required field: MessageId, is not set");
    return GetChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MessageId]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessage", "Required field: ChimeBearer, is not set");
    return GetChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMessageId());
  return GetChannelMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetChannelMessageOutcomeCallable ChimeSDKMessagingClient::GetChannelMessageCallable(const GetChannelMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::GetChannelMessage, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::GetChannelMessageAsync(const GetChannelMessageRequest& request, const GetChannelMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::GetChannelMessage, this, request, handler, context, m_executor.get());
}

GetChannelMessageStatusOutcome ChimeSDKMessagingClient::GetChannelMessageStatus(const GetChannelMessageStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChannelMessageStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessageStatus", "Required field: ChannelArn, is not set");
    return GetChannelMessageStatusOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MessageIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessageStatus", "Required field: MessageId, is not set");
    return GetChannelMessageStatusOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MessageId]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetChannelMessageStatus", "Required field: ChimeBearer, is not set");
    return GetChannelMessageStatusOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChannelMessageStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMessageId());
  ss.str("?scope=message-status");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return GetChannelMessageStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetChannelMessageStatusOutcomeCallable ChimeSDKMessagingClient::GetChannelMessageStatusCallable(const GetChannelMessageStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::GetChannelMessageStatus, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::GetChannelMessageStatusAsync(const GetChannelMessageStatusRequest& request, const GetChannelMessageStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::GetChannelMessageStatus, this, request, handler, context, m_executor.get());
}

GetMessagingSessionEndpointOutcome ChimeSDKMessagingClient::GetMessagingSessionEndpoint(const GetMessagingSessionEndpointRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMessagingSessionEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMessagingSessionEndpoint, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/endpoints/messaging-session");
  return GetMessagingSessionEndpointOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMessagingSessionEndpointOutcomeCallable ChimeSDKMessagingClient::GetMessagingSessionEndpointCallable(const GetMessagingSessionEndpointRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::GetMessagingSessionEndpoint, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::GetMessagingSessionEndpointAsync(const GetMessagingSessionEndpointRequest& request, const GetMessagingSessionEndpointResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::GetMessagingSessionEndpoint, this, request, handler, context, m_executor.get());
}

ListChannelBansOutcome ChimeSDKMessagingClient::ListChannelBans(const ListChannelBansRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelBans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelBans", "Required field: ChannelArn, is not set");
    return ListChannelBansOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelBans", "Required field: ChimeBearer, is not set");
    return ListChannelBansOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelBans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/bans");
  return ListChannelBansOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelBansOutcomeCallable ChimeSDKMessagingClient::ListChannelBansCallable(const ListChannelBansRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelBans, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelBansAsync(const ListChannelBansRequest& request, const ListChannelBansResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelBans, this, request, handler, context, m_executor.get());
}

ListChannelFlowsOutcome ChimeSDKMessagingClient::ListChannelFlows(const ListChannelFlowsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelFlows, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppInstanceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelFlows", "Required field: AppInstanceArn, is not set");
    return ListChannelFlowsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppInstanceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelFlows, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flows");
  return ListChannelFlowsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelFlowsOutcomeCallable ChimeSDKMessagingClient::ListChannelFlowsCallable(const ListChannelFlowsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelFlows, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelFlowsAsync(const ListChannelFlowsRequest& request, const ListChannelFlowsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelFlows, this, request, handler, context, m_executor.get());
}

ListChannelMembershipsOutcome ChimeSDKMessagingClient::ListChannelMemberships(const ListChannelMembershipsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelMemberships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelMemberships", "Required field: ChannelArn, is not set");
    return ListChannelMembershipsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelMemberships", "Required field: ChimeBearer, is not set");
    return ListChannelMembershipsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelMemberships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships");
  return ListChannelMembershipsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelMembershipsOutcomeCallable ChimeSDKMessagingClient::ListChannelMembershipsCallable(const ListChannelMembershipsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelMemberships, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelMembershipsAsync(const ListChannelMembershipsRequest& request, const ListChannelMembershipsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelMemberships, this, request, handler, context, m_executor.get());
}

ListChannelMembershipsForAppInstanceUserOutcome ChimeSDKMessagingClient::ListChannelMembershipsForAppInstanceUser(const ListChannelMembershipsForAppInstanceUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelMembershipsForAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelMembershipsForAppInstanceUser", "Required field: ChimeBearer, is not set");
    return ListChannelMembershipsForAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelMembershipsForAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  ss.str("?scope=app-instance-user-memberships");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ListChannelMembershipsForAppInstanceUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelMembershipsForAppInstanceUserOutcomeCallable ChimeSDKMessagingClient::ListChannelMembershipsForAppInstanceUserCallable(const ListChannelMembershipsForAppInstanceUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelMembershipsForAppInstanceUser, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelMembershipsForAppInstanceUserAsync(const ListChannelMembershipsForAppInstanceUserRequest& request, const ListChannelMembershipsForAppInstanceUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelMembershipsForAppInstanceUser, this, request, handler, context, m_executor.get());
}

ListChannelMessagesOutcome ChimeSDKMessagingClient::ListChannelMessages(const ListChannelMessagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelMessages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelMessages", "Required field: ChannelArn, is not set");
    return ListChannelMessagesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelMessages", "Required field: ChimeBearer, is not set");
    return ListChannelMessagesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelMessages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages");
  return ListChannelMessagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelMessagesOutcomeCallable ChimeSDKMessagingClient::ListChannelMessagesCallable(const ListChannelMessagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelMessages, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelMessagesAsync(const ListChannelMessagesRequest& request, const ListChannelMessagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelMessages, this, request, handler, context, m_executor.get());
}

ListChannelModeratorsOutcome ChimeSDKMessagingClient::ListChannelModerators(const ListChannelModeratorsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelModerators, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelModerators", "Required field: ChannelArn, is not set");
    return ListChannelModeratorsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelModerators", "Required field: ChimeBearer, is not set");
    return ListChannelModeratorsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelModerators, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/moderators");
  return ListChannelModeratorsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelModeratorsOutcomeCallable ChimeSDKMessagingClient::ListChannelModeratorsCallable(const ListChannelModeratorsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelModerators, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelModeratorsAsync(const ListChannelModeratorsRequest& request, const ListChannelModeratorsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelModerators, this, request, handler, context, m_executor.get());
}

ListChannelsOutcome ChimeSDKMessagingClient::ListChannels(const ListChannelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppInstanceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannels", "Required field: AppInstanceArn, is not set");
    return ListChannelsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppInstanceArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannels", "Required field: ChimeBearer, is not set");
    return ListChannelsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  return ListChannelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelsOutcomeCallable ChimeSDKMessagingClient::ListChannelsCallable(const ListChannelsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannels, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelsAsync(const ListChannelsRequest& request, const ListChannelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannels, this, request, handler, context, m_executor.get());
}

ListChannelsAssociatedWithChannelFlowOutcome ChimeSDKMessagingClient::ListChannelsAssociatedWithChannelFlow(const ListChannelsAssociatedWithChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelsAssociatedWithChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelFlowArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelsAssociatedWithChannelFlow", "Required field: ChannelFlowArn, is not set");
    return ListChannelsAssociatedWithChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelFlowArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelsAssociatedWithChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  ss.str("?scope=channel-flow-associations");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ListChannelsAssociatedWithChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelsAssociatedWithChannelFlowOutcomeCallable ChimeSDKMessagingClient::ListChannelsAssociatedWithChannelFlowCallable(const ListChannelsAssociatedWithChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelsAssociatedWithChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelsAssociatedWithChannelFlowAsync(const ListChannelsAssociatedWithChannelFlowRequest& request, const ListChannelsAssociatedWithChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelsAssociatedWithChannelFlow, this, request, handler, context, m_executor.get());
}

ListChannelsModeratedByAppInstanceUserOutcome ChimeSDKMessagingClient::ListChannelsModeratedByAppInstanceUser(const ListChannelsModeratedByAppInstanceUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListChannelsModeratedByAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListChannelsModeratedByAppInstanceUser", "Required field: ChimeBearer, is not set");
    return ListChannelsModeratedByAppInstanceUserOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListChannelsModeratedByAppInstanceUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  ss.str("?scope=app-instance-user-moderated-channels");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return ListChannelsModeratedByAppInstanceUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListChannelsModeratedByAppInstanceUserOutcomeCallable ChimeSDKMessagingClient::ListChannelsModeratedByAppInstanceUserCallable(const ListChannelsModeratedByAppInstanceUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListChannelsModeratedByAppInstanceUser, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListChannelsModeratedByAppInstanceUserAsync(const ListChannelsModeratedByAppInstanceUserRequest& request, const ListChannelsModeratedByAppInstanceUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListChannelsModeratedByAppInstanceUser, this, request, handler, context, m_executor.get());
}

ListSubChannelsOutcome ChimeSDKMessagingClient::ListSubChannels(const ListSubChannelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSubChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSubChannels", "Required field: ChannelArn, is not set");
    return ListSubChannelsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSubChannels", "Required field: ChimeBearer, is not set");
    return ListSubChannelsOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSubChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/subchannels");
  return ListSubChannelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSubChannelsOutcomeCallable ChimeSDKMessagingClient::ListSubChannelsCallable(const ListSubChannelsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListSubChannels, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListSubChannelsAsync(const ListSubChannelsRequest& request, const ListSubChannelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListSubChannels, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome ChimeSDKMessagingClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceARNHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceARN, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceARN]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable ChimeSDKMessagingClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::ListTagsForResource, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

PutChannelMembershipPreferencesOutcome ChimeSDKMessagingClient::PutChannelMembershipPreferences(const PutChannelMembershipPreferencesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutChannelMembershipPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutChannelMembershipPreferences", "Required field: ChannelArn, is not set");
    return PutChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MemberArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutChannelMembershipPreferences", "Required field: MemberArn, is not set");
    return PutChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MemberArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutChannelMembershipPreferences", "Required field: ChimeBearer, is not set");
    return PutChannelMembershipPreferencesOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutChannelMembershipPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/memberships/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMemberArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/preferences");
  return PutChannelMembershipPreferencesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutChannelMembershipPreferencesOutcomeCallable ChimeSDKMessagingClient::PutChannelMembershipPreferencesCallable(const PutChannelMembershipPreferencesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::PutChannelMembershipPreferences, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::PutChannelMembershipPreferencesAsync(const PutChannelMembershipPreferencesRequest& request, const PutChannelMembershipPreferencesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::PutChannelMembershipPreferences, this, request, handler, context, m_executor.get());
}

RedactChannelMessageOutcome ChimeSDKMessagingClient::RedactChannelMessage(const RedactChannelMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RedactChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RedactChannelMessage", "Required field: ChannelArn, is not set");
    return RedactChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MessageIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RedactChannelMessage", "Required field: MessageId, is not set");
    return RedactChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MessageId]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RedactChannelMessage", "Required field: ChimeBearer, is not set");
    return RedactChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RedactChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMessageId());
  ss.str("?operation=redact");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return RedactChannelMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RedactChannelMessageOutcomeCallable ChimeSDKMessagingClient::RedactChannelMessageCallable(const RedactChannelMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::RedactChannelMessage, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::RedactChannelMessageAsync(const RedactChannelMessageRequest& request, const RedactChannelMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::RedactChannelMessage, this, request, handler, context, m_executor.get());
}

SearchChannelsOutcome ChimeSDKMessagingClient::SearchChannels(const SearchChannelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SearchChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SearchChannels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels");
  ss.str("?operation=search");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return SearchChannelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SearchChannelsOutcomeCallable ChimeSDKMessagingClient::SearchChannelsCallable(const SearchChannelsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::SearchChannels, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::SearchChannelsAsync(const SearchChannelsRequest& request, const SearchChannelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::SearchChannels, this, request, handler, context, m_executor.get());
}

SendChannelMessageOutcome ChimeSDKMessagingClient::SendChannelMessage(const SendChannelMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SendChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SendChannelMessage", "Required field: ChannelArn, is not set");
    return SendChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SendChannelMessage", "Required field: ChimeBearer, is not set");
    return SendChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SendChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages");
  return SendChannelMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SendChannelMessageOutcomeCallable ChimeSDKMessagingClient::SendChannelMessageCallable(const SendChannelMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::SendChannelMessage, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::SendChannelMessageAsync(const SendChannelMessageRequest& request, const SendChannelMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::SendChannelMessage, this, request, handler, context, m_executor.get());
}

TagResourceOutcome ChimeSDKMessagingClient::TagResource(const TagResourceRequest& request) const
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

TagResourceOutcomeCallable ChimeSDKMessagingClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::TagResource, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome ChimeSDKMessagingClient::UntagResource(const UntagResourceRequest& request) const
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

UntagResourceOutcomeCallable ChimeSDKMessagingClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::UntagResource, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateChannelOutcome ChimeSDKMessagingClient::UpdateChannel(const UpdateChannelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannel", "Required field: ChannelArn, is not set");
    return UpdateChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannel", "Required field: ChimeBearer, is not set");
    return UpdateChannelOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateChannel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  return UpdateChannelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateChannelOutcomeCallable ChimeSDKMessagingClient::UpdateChannelCallable(const UpdateChannelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::UpdateChannel, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::UpdateChannelAsync(const UpdateChannelRequest& request, const UpdateChannelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::UpdateChannel, this, request, handler, context, m_executor.get());
}

UpdateChannelFlowOutcome ChimeSDKMessagingClient::UpdateChannelFlow(const UpdateChannelFlowRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelFlowArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelFlow", "Required field: ChannelFlowArn, is not set");
    return UpdateChannelFlowOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelFlowArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateChannelFlow, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channel-flows/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelFlowArn());
  return UpdateChannelFlowOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateChannelFlowOutcomeCallable ChimeSDKMessagingClient::UpdateChannelFlowCallable(const UpdateChannelFlowRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::UpdateChannelFlow, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::UpdateChannelFlowAsync(const UpdateChannelFlowRequest& request, const UpdateChannelFlowResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::UpdateChannelFlow, this, request, handler, context, m_executor.get());
}

UpdateChannelMessageOutcome ChimeSDKMessagingClient::UpdateChannelMessage(const UpdateChannelMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelMessage", "Required field: ChannelArn, is not set");
    return UpdateChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.MessageIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelMessage", "Required field: MessageId, is not set");
    return UpdateChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MessageId]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelMessage", "Required field: ChimeBearer, is not set");
    return UpdateChannelMessageOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateChannelMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/messages/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMessageId());
  return UpdateChannelMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateChannelMessageOutcomeCallable ChimeSDKMessagingClient::UpdateChannelMessageCallable(const UpdateChannelMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::UpdateChannelMessage, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::UpdateChannelMessageAsync(const UpdateChannelMessageRequest& request, const UpdateChannelMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::UpdateChannelMessage, this, request, handler, context, m_executor.get());
}

UpdateChannelReadMarkerOutcome ChimeSDKMessagingClient::UpdateChannelReadMarker(const UpdateChannelReadMarkerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateChannelReadMarker, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ChannelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelReadMarker", "Required field: ChannelArn, is not set");
    return UpdateChannelReadMarkerOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChannelArn]", false));
  }
  if (!request.ChimeBearerHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateChannelReadMarker", "Required field: ChimeBearer, is not set");
    return UpdateChannelReadMarkerOutcome(Aws::Client::AWSError<ChimeSDKMessagingErrors>(ChimeSDKMessagingErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ChimeBearer]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateChannelReadMarker, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/channels/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetChannelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/readMarker");
  return UpdateChannelReadMarkerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateChannelReadMarkerOutcomeCallable ChimeSDKMessagingClient::UpdateChannelReadMarkerCallable(const UpdateChannelReadMarkerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ChimeSDKMessagingClient::UpdateChannelReadMarker, this, request, m_executor.get());
}

void ChimeSDKMessagingClient::UpdateChannelReadMarkerAsync(const UpdateChannelReadMarkerRequest& request, const UpdateChannelReadMarkerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ChimeSDKMessagingClient::UpdateChannelReadMarker, this, request, handler, context, m_executor.get());
}

