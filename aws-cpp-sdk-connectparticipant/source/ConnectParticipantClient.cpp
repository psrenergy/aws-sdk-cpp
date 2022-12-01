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

#include <aws/connectparticipant/ConnectParticipantClient.h>
#include <aws/connectparticipant/ConnectParticipantErrorMarshaller.h>
#include <aws/connectparticipant/ConnectParticipantEndpointProvider.h>
#include <aws/connectparticipant/model/CompleteAttachmentUploadRequest.h>
#include <aws/connectparticipant/model/CreateParticipantConnectionRequest.h>
#include <aws/connectparticipant/model/DisconnectParticipantRequest.h>
#include <aws/connectparticipant/model/GetAttachmentRequest.h>
#include <aws/connectparticipant/model/GetTranscriptRequest.h>
#include <aws/connectparticipant/model/SendEventRequest.h>
#include <aws/connectparticipant/model/SendMessageRequest.h>
#include <aws/connectparticipant/model/StartAttachmentUploadRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ConnectParticipant;
using namespace Aws::ConnectParticipant::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ConnectParticipantClient::SERVICE_NAME = "execute-api";
const char* ConnectParticipantClient::ALLOCATION_TAG = "ConnectParticipantClient";

ConnectParticipantClient::ConnectParticipantClient(const ConnectParticipant::ConnectParticipantClientConfiguration& clientConfiguration,
                                                   std::shared_ptr<ConnectParticipantEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ConnectParticipantClient::ConnectParticipantClient(const AWSCredentials& credentials,
                                                   std::shared_ptr<ConnectParticipantEndpointProviderBase> endpointProvider,
                                                   const ConnectParticipant::ConnectParticipantClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ConnectParticipantClient::ConnectParticipantClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                   std::shared_ptr<ConnectParticipantEndpointProviderBase> endpointProvider,
                                                   const ConnectParticipant::ConnectParticipantClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ConnectParticipantClient::ConnectParticipantClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ConnectParticipantEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ConnectParticipantClient::ConnectParticipantClient(const AWSCredentials& credentials,
                                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ConnectParticipantEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ConnectParticipantClient::ConnectParticipantClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectParticipantErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ConnectParticipantEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ConnectParticipantClient::~ConnectParticipantClient()
{
}

std::shared_ptr<ConnectParticipantEndpointProviderBase>& ConnectParticipantClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ConnectParticipantClient::init(const ConnectParticipant::ConnectParticipantClientConfiguration& config)
{
  AWSClient::SetServiceClientName("ConnectParticipant");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ConnectParticipantClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CompleteAttachmentUploadOutcome ConnectParticipantClient::CompleteAttachmentUpload(const CompleteAttachmentUploadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CompleteAttachmentUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CompleteAttachmentUpload", "Required field: ConnectionToken, is not set");
    return CompleteAttachmentUploadOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CompleteAttachmentUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/complete-attachment-upload");
  return CompleteAttachmentUploadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CompleteAttachmentUploadOutcomeCallable ConnectParticipantClient::CompleteAttachmentUploadCallable(const CompleteAttachmentUploadRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::CompleteAttachmentUpload, this, request, m_executor.get());
}

void ConnectParticipantClient::CompleteAttachmentUploadAsync(const CompleteAttachmentUploadRequest& request, const CompleteAttachmentUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::CompleteAttachmentUpload, this, request, handler, context, m_executor.get());
}

CreateParticipantConnectionOutcome ConnectParticipantClient::CreateParticipantConnection(const CreateParticipantConnectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateParticipantConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ParticipantTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateParticipantConnection", "Required field: ParticipantToken, is not set");
    return CreateParticipantConnectionOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ParticipantToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateParticipantConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/connection");
  return CreateParticipantConnectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateParticipantConnectionOutcomeCallable ConnectParticipantClient::CreateParticipantConnectionCallable(const CreateParticipantConnectionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::CreateParticipantConnection, this, request, m_executor.get());
}

void ConnectParticipantClient::CreateParticipantConnectionAsync(const CreateParticipantConnectionRequest& request, const CreateParticipantConnectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::CreateParticipantConnection, this, request, handler, context, m_executor.get());
}

DisconnectParticipantOutcome ConnectParticipantClient::DisconnectParticipant(const DisconnectParticipantRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisconnectParticipant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisconnectParticipant", "Required field: ConnectionToken, is not set");
    return DisconnectParticipantOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisconnectParticipant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/disconnect");
  return DisconnectParticipantOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisconnectParticipantOutcomeCallable ConnectParticipantClient::DisconnectParticipantCallable(const DisconnectParticipantRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::DisconnectParticipant, this, request, m_executor.get());
}

void ConnectParticipantClient::DisconnectParticipantAsync(const DisconnectParticipantRequest& request, const DisconnectParticipantResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::DisconnectParticipant, this, request, handler, context, m_executor.get());
}

GetAttachmentOutcome ConnectParticipantClient::GetAttachment(const GetAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAttachment", "Required field: ConnectionToken, is not set");
    return GetAttachmentOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/attachment");
  return GetAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetAttachmentOutcomeCallable ConnectParticipantClient::GetAttachmentCallable(const GetAttachmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::GetAttachment, this, request, m_executor.get());
}

void ConnectParticipantClient::GetAttachmentAsync(const GetAttachmentRequest& request, const GetAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::GetAttachment, this, request, handler, context, m_executor.get());
}

GetTranscriptOutcome ConnectParticipantClient::GetTranscript(const GetTranscriptRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTranscript, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTranscript", "Required field: ConnectionToken, is not set");
    return GetTranscriptOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTranscript, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/transcript");
  return GetTranscriptOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetTranscriptOutcomeCallable ConnectParticipantClient::GetTranscriptCallable(const GetTranscriptRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::GetTranscript, this, request, m_executor.get());
}

void ConnectParticipantClient::GetTranscriptAsync(const GetTranscriptRequest& request, const GetTranscriptResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::GetTranscript, this, request, handler, context, m_executor.get());
}

SendEventOutcome ConnectParticipantClient::SendEvent(const SendEventRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SendEvent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SendEvent", "Required field: ConnectionToken, is not set");
    return SendEventOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SendEvent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/event");
  return SendEventOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SendEventOutcomeCallable ConnectParticipantClient::SendEventCallable(const SendEventRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::SendEvent, this, request, m_executor.get());
}

void ConnectParticipantClient::SendEventAsync(const SendEventRequest& request, const SendEventResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::SendEvent, this, request, handler, context, m_executor.get());
}

SendMessageOutcome ConnectParticipantClient::SendMessage(const SendMessageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SendMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("SendMessage", "Required field: ConnectionToken, is not set");
    return SendMessageOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SendMessage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/message");
  return SendMessageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SendMessageOutcomeCallable ConnectParticipantClient::SendMessageCallable(const SendMessageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::SendMessage, this, request, m_executor.get());
}

void ConnectParticipantClient::SendMessageAsync(const SendMessageRequest& request, const SendMessageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::SendMessage, this, request, handler, context, m_executor.get());
}

StartAttachmentUploadOutcome ConnectParticipantClient::StartAttachmentUpload(const StartAttachmentUploadRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartAttachmentUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectionTokenHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartAttachmentUpload", "Required field: ConnectionToken, is not set");
    return StartAttachmentUploadOutcome(Aws::Client::AWSError<ConnectParticipantErrors>(ConnectParticipantErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionToken]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartAttachmentUpload, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/participant/start-attachment-upload");
  return StartAttachmentUploadOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartAttachmentUploadOutcomeCallable ConnectParticipantClient::StartAttachmentUploadCallable(const StartAttachmentUploadRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ConnectParticipantClient::StartAttachmentUpload, this, request, m_executor.get());
}

void ConnectParticipantClient::StartAttachmentUploadAsync(const StartAttachmentUploadRequest& request, const StartAttachmentUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ConnectParticipantClient::StartAttachmentUpload, this, request, handler, context, m_executor.get());
}

