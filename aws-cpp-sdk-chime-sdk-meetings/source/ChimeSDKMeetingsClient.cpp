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

#include <aws/chime-sdk-meetings/ChimeSDKMeetingsClient.h>
#include <aws/chime-sdk-meetings/ChimeSDKMeetingsErrorMarshaller.h>
#include <aws/chime-sdk-meetings/ChimeSDKMeetingsEndpointProvider.h>
#include <aws/chime-sdk-meetings/model/BatchCreateAttendeeRequest.h>
#include <aws/chime-sdk-meetings/model/BatchUpdateAttendeeCapabilitiesExceptRequest.h>
#include <aws/chime-sdk-meetings/model/CreateAttendeeRequest.h>
#include <aws/chime-sdk-meetings/model/CreateMeetingRequest.h>
#include <aws/chime-sdk-meetings/model/CreateMeetingWithAttendeesRequest.h>
#include <aws/chime-sdk-meetings/model/DeleteAttendeeRequest.h>
#include <aws/chime-sdk-meetings/model/DeleteMeetingRequest.h>
#include <aws/chime-sdk-meetings/model/GetAttendeeRequest.h>
#include <aws/chime-sdk-meetings/model/GetMeetingRequest.h>
#include <aws/chime-sdk-meetings/model/ListAttendeesRequest.h>
#include <aws/chime-sdk-meetings/model/ListTagsForResourceRequest.h>
#include <aws/chime-sdk-meetings/model/StartMeetingTranscriptionRequest.h>
#include <aws/chime-sdk-meetings/model/StopMeetingTranscriptionRequest.h>
#include <aws/chime-sdk-meetings/model/TagResourceRequest.h>
#include <aws/chime-sdk-meetings/model/UntagResourceRequest.h>
#include <aws/chime-sdk-meetings/model/UpdateAttendeeCapabilitiesRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ChimeSDKMeetings;
using namespace Aws::ChimeSDKMeetings::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ChimeSDKMeetingsClient::SERVICE_NAME = "chime";
const char* ChimeSDKMeetingsClient::ALLOCATION_TAG = "ChimeSDKMeetingsClient";

ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const ChimeSDKMeetings::ChimeSDKMeetingsClientConfiguration& clientConfiguration,
                                               std::shared_ptr<ChimeSDKMeetingsEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const AWSCredentials& credentials,
                                               std::shared_ptr<ChimeSDKMeetingsEndpointProviderBase> endpointProvider,
                                               const ChimeSDKMeetings::ChimeSDKMeetingsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               std::shared_ptr<ChimeSDKMeetingsEndpointProviderBase> endpointProvider,
                                               const ChimeSDKMeetings::ChimeSDKMeetingsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ChimeSDKMeetingsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const AWSCredentials& credentials,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMeetingsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ChimeSDKMeetingsClient::ChimeSDKMeetingsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ChimeSDKMeetingsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ChimeSDKMeetingsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ChimeSDKMeetingsClient::~ChimeSDKMeetingsClient()
{
}

std::shared_ptr<ChimeSDKMeetingsEndpointProviderBase>& ChimeSDKMeetingsClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ChimeSDKMeetingsClient::init(const ChimeSDKMeetings::ChimeSDKMeetingsClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Chime SDK Meetings");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ChimeSDKMeetingsClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchCreateAttendeeOutcome ChimeSDKMeetingsClient::BatchCreateAttendee(const BatchCreateAttendeeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchCreateAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchCreateAttendee", "Required field: MeetingId, is not set");
    return BatchCreateAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchCreateAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees");
  ss.str("?operation=batch-create");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return BatchCreateAttendeeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchCreateAttendeeOutcomeCallable ChimeSDKMeetingsClient::BatchCreateAttendeeCallable(const BatchCreateAttendeeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchCreateAttendee, request, m_executor.get());
}

void ChimeSDKMeetingsClient::BatchCreateAttendeeAsync(const BatchCreateAttendeeRequest& request, const BatchCreateAttendeeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchCreateAttendee, request, handler, context, m_executor.get());
}

BatchUpdateAttendeeCapabilitiesExceptOutcome ChimeSDKMeetingsClient::BatchUpdateAttendeeCapabilitiesExcept(const BatchUpdateAttendeeCapabilitiesExceptRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchUpdateAttendeeCapabilitiesExcept, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchUpdateAttendeeCapabilitiesExcept", "Required field: MeetingId, is not set");
    return BatchUpdateAttendeeCapabilitiesExceptOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchUpdateAttendeeCapabilitiesExcept, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees/capabilities");
  ss.str("?operation=batch-update-except");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return BatchUpdateAttendeeCapabilitiesExceptOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

BatchUpdateAttendeeCapabilitiesExceptOutcomeCallable ChimeSDKMeetingsClient::BatchUpdateAttendeeCapabilitiesExceptCallable(const BatchUpdateAttendeeCapabilitiesExceptRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchUpdateAttendeeCapabilitiesExcept, request, m_executor.get());
}

void ChimeSDKMeetingsClient::BatchUpdateAttendeeCapabilitiesExceptAsync(const BatchUpdateAttendeeCapabilitiesExceptRequest& request, const BatchUpdateAttendeeCapabilitiesExceptResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchUpdateAttendeeCapabilitiesExcept, request, handler, context, m_executor.get());
}

CreateAttendeeOutcome ChimeSDKMeetingsClient::CreateAttendee(const CreateAttendeeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateAttendee", "Required field: MeetingId, is not set");
    return CreateAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees");
  return CreateAttendeeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAttendeeOutcomeCallable ChimeSDKMeetingsClient::CreateAttendeeCallable(const CreateAttendeeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAttendee, request, m_executor.get());
}

void ChimeSDKMeetingsClient::CreateAttendeeAsync(const CreateAttendeeRequest& request, const CreateAttendeeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAttendee, request, handler, context, m_executor.get());
}

CreateMeetingOutcome ChimeSDKMeetingsClient::CreateMeeting(const CreateMeetingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings");
  return CreateMeetingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMeetingOutcomeCallable ChimeSDKMeetingsClient::CreateMeetingCallable(const CreateMeetingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateMeeting, request, m_executor.get());
}

void ChimeSDKMeetingsClient::CreateMeetingAsync(const CreateMeetingRequest& request, const CreateMeetingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateMeeting, request, handler, context, m_executor.get());
}

CreateMeetingWithAttendeesOutcome ChimeSDKMeetingsClient::CreateMeetingWithAttendees(const CreateMeetingWithAttendeesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMeetingWithAttendees, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMeetingWithAttendees, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings");
  ss.str("?operation=create-attendees");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return CreateMeetingWithAttendeesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMeetingWithAttendeesOutcomeCallable ChimeSDKMeetingsClient::CreateMeetingWithAttendeesCallable(const CreateMeetingWithAttendeesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateMeetingWithAttendees, request, m_executor.get());
}

void ChimeSDKMeetingsClient::CreateMeetingWithAttendeesAsync(const CreateMeetingWithAttendeesRequest& request, const CreateMeetingWithAttendeesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateMeetingWithAttendees, request, handler, context, m_executor.get());
}

DeleteAttendeeOutcome ChimeSDKMeetingsClient::DeleteAttendee(const DeleteAttendeeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAttendee", "Required field: MeetingId, is not set");
    return DeleteAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  if (!request.AttendeeIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAttendee", "Required field: AttendeeId, is not set");
    return DeleteAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttendeeId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttendeeId());
  return DeleteAttendeeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAttendeeOutcomeCallable ChimeSDKMeetingsClient::DeleteAttendeeCallable(const DeleteAttendeeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAttendee, request, m_executor.get());
}

void ChimeSDKMeetingsClient::DeleteAttendeeAsync(const DeleteAttendeeRequest& request, const DeleteAttendeeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAttendee, request, handler, context, m_executor.get());
}

DeleteMeetingOutcome ChimeSDKMeetingsClient::DeleteMeeting(const DeleteMeetingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteMeeting", "Required field: MeetingId, is not set");
    return DeleteMeetingOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  return DeleteMeetingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteMeetingOutcomeCallable ChimeSDKMeetingsClient::DeleteMeetingCallable(const DeleteMeetingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteMeeting, request, m_executor.get());
}

void ChimeSDKMeetingsClient::DeleteMeetingAsync(const DeleteMeetingRequest& request, const DeleteMeetingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteMeeting, request, handler, context, m_executor.get());
}

GetAttendeeOutcome ChimeSDKMeetingsClient::GetAttendee(const GetAttendeeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAttendee", "Required field: MeetingId, is not set");
    return GetAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  if (!request.AttendeeIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAttendee", "Required field: AttendeeId, is not set");
    return GetAttendeeOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttendeeId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAttendee, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttendeeId());
  return GetAttendeeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAttendeeOutcomeCallable ChimeSDKMeetingsClient::GetAttendeeCallable(const GetAttendeeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetAttendee, request, m_executor.get());
}

void ChimeSDKMeetingsClient::GetAttendeeAsync(const GetAttendeeRequest& request, const GetAttendeeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetAttendee, request, handler, context, m_executor.get());
}

GetMeetingOutcome ChimeSDKMeetingsClient::GetMeeting(const GetMeetingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMeeting", "Required field: MeetingId, is not set");
    return GetMeetingOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMeeting, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  return GetMeetingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMeetingOutcomeCallable ChimeSDKMeetingsClient::GetMeetingCallable(const GetMeetingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetMeeting, request, m_executor.get());
}

void ChimeSDKMeetingsClient::GetMeetingAsync(const GetMeetingRequest& request, const GetMeetingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetMeeting, request, handler, context, m_executor.get());
}

ListAttendeesOutcome ChimeSDKMeetingsClient::ListAttendees(const ListAttendeesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAttendees, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAttendees", "Required field: MeetingId, is not set");
    return ListAttendeesOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAttendees, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees");
  return ListAttendeesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAttendeesOutcomeCallable ChimeSDKMeetingsClient::ListAttendeesCallable(const ListAttendeesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAttendees, request, m_executor.get());
}

void ChimeSDKMeetingsClient::ListAttendeesAsync(const ListAttendeesRequest& request, const ListAttendeesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAttendees, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome ChimeSDKMeetingsClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceARNHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceARN, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceARN]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable ChimeSDKMeetingsClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void ChimeSDKMeetingsClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

StartMeetingTranscriptionOutcome ChimeSDKMeetingsClient::StartMeetingTranscription(const StartMeetingTranscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartMeetingTranscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartMeetingTranscription", "Required field: MeetingId, is not set");
    return StartMeetingTranscriptionOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartMeetingTranscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transcription");
  ss.str("?operation=start");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return StartMeetingTranscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartMeetingTranscriptionOutcomeCallable ChimeSDKMeetingsClient::StartMeetingTranscriptionCallable(const StartMeetingTranscriptionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartMeetingTranscription, request, m_executor.get());
}

void ChimeSDKMeetingsClient::StartMeetingTranscriptionAsync(const StartMeetingTranscriptionRequest& request, const StartMeetingTranscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartMeetingTranscription, request, handler, context, m_executor.get());
}

StopMeetingTranscriptionOutcome ChimeSDKMeetingsClient::StopMeetingTranscription(const StopMeetingTranscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopMeetingTranscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopMeetingTranscription", "Required field: MeetingId, is not set");
    return StopMeetingTranscriptionOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopMeetingTranscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  Aws::StringStream ss;
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transcription");
  ss.str("?operation=stop");
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());
  return StopMeetingTranscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopMeetingTranscriptionOutcomeCallable ChimeSDKMeetingsClient::StopMeetingTranscriptionCallable(const StopMeetingTranscriptionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StopMeetingTranscription, request, m_executor.get());
}

void ChimeSDKMeetingsClient::StopMeetingTranscriptionAsync(const StopMeetingTranscriptionRequest& request, const StopMeetingTranscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StopMeetingTranscription, request, handler, context, m_executor.get());
}

TagResourceOutcome ChimeSDKMeetingsClient::TagResource(const TagResourceRequest& request) const
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

TagResourceOutcomeCallable ChimeSDKMeetingsClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void ChimeSDKMeetingsClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome ChimeSDKMeetingsClient::UntagResource(const UntagResourceRequest& request) const
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

UntagResourceOutcomeCallable ChimeSDKMeetingsClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void ChimeSDKMeetingsClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateAttendeeCapabilitiesOutcome ChimeSDKMeetingsClient::UpdateAttendeeCapabilities(const UpdateAttendeeCapabilitiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAttendeeCapabilities, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.MeetingIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAttendeeCapabilities", "Required field: MeetingId, is not set");
    return UpdateAttendeeCapabilitiesOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [MeetingId]", false));
  }
  if (!request.AttendeeIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAttendeeCapabilities", "Required field: AttendeeId, is not set");
    return UpdateAttendeeCapabilitiesOutcome(Aws::Client::AWSError<ChimeSDKMeetingsErrors>(ChimeSDKMeetingsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttendeeId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAttendeeCapabilities, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/meetings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetMeetingId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attendees/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttendeeId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/capabilities");
  return UpdateAttendeeCapabilitiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateAttendeeCapabilitiesOutcomeCallable ChimeSDKMeetingsClient::UpdateAttendeeCapabilitiesCallable(const UpdateAttendeeCapabilitiesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAttendeeCapabilities, request, m_executor.get());
}

void ChimeSDKMeetingsClient::UpdateAttendeeCapabilitiesAsync(const UpdateAttendeeCapabilitiesRequest& request, const UpdateAttendeeCapabilitiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAttendeeCapabilities, request, handler, context, m_executor.get());
}

