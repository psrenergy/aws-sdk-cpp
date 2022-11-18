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

#include <aws/connectcampaigns/ConnectCampaignsClient.h>
#include <aws/connectcampaigns/ConnectCampaignsErrorMarshaller.h>
#include <aws/connectcampaigns/ConnectCampaignsEndpointProvider.h>
#include <aws/connectcampaigns/model/CreateCampaignRequest.h>
#include <aws/connectcampaigns/model/DeleteCampaignRequest.h>
#include <aws/connectcampaigns/model/DeleteConnectInstanceConfigRequest.h>
#include <aws/connectcampaigns/model/DeleteInstanceOnboardingJobRequest.h>
#include <aws/connectcampaigns/model/DescribeCampaignRequest.h>
#include <aws/connectcampaigns/model/GetCampaignStateRequest.h>
#include <aws/connectcampaigns/model/GetCampaignStateBatchRequest.h>
#include <aws/connectcampaigns/model/GetConnectInstanceConfigRequest.h>
#include <aws/connectcampaigns/model/GetInstanceOnboardingJobStatusRequest.h>
#include <aws/connectcampaigns/model/ListCampaignsRequest.h>
#include <aws/connectcampaigns/model/ListTagsForResourceRequest.h>
#include <aws/connectcampaigns/model/PauseCampaignRequest.h>
#include <aws/connectcampaigns/model/PutDialRequestBatchRequest.h>
#include <aws/connectcampaigns/model/ResumeCampaignRequest.h>
#include <aws/connectcampaigns/model/StartCampaignRequest.h>
#include <aws/connectcampaigns/model/StartInstanceOnboardingJobRequest.h>
#include <aws/connectcampaigns/model/StopCampaignRequest.h>
#include <aws/connectcampaigns/model/TagResourceRequest.h>
#include <aws/connectcampaigns/model/UntagResourceRequest.h>
#include <aws/connectcampaigns/model/UpdateCampaignDialerConfigRequest.h>
#include <aws/connectcampaigns/model/UpdateCampaignNameRequest.h>
#include <aws/connectcampaigns/model/UpdateCampaignOutboundCallConfigRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ConnectCampaigns;
using namespace Aws::ConnectCampaigns::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ConnectCampaignsClient::SERVICE_NAME = "connect-campaigns";
const char* ConnectCampaignsClient::ALLOCATION_TAG = "ConnectCampaignsClient";

ConnectCampaignsClient::ConnectCampaignsClient(const ConnectCampaigns::ConnectCampaignsClientConfiguration& clientConfiguration,
                                               std::shared_ptr<ConnectCampaignsEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ConnectCampaignsClient::ConnectCampaignsClient(const AWSCredentials& credentials,
                                               std::shared_ptr<ConnectCampaignsEndpointProviderBase> endpointProvider,
                                               const ConnectCampaigns::ConnectCampaignsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ConnectCampaignsClient::ConnectCampaignsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               std::shared_ptr<ConnectCampaignsEndpointProviderBase> endpointProvider,
                                               const ConnectCampaigns::ConnectCampaignsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ConnectCampaignsClient::ConnectCampaignsClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ConnectCampaignsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ConnectCampaignsClient::ConnectCampaignsClient(const AWSCredentials& credentials,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ConnectCampaignsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ConnectCampaignsClient::ConnectCampaignsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ConnectCampaignsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ConnectCampaignsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ConnectCampaignsClient::~ConnectCampaignsClient()
{
}

std::shared_ptr<ConnectCampaignsEndpointProviderBase>& ConnectCampaignsClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ConnectCampaignsClient::init(const ConnectCampaigns::ConnectCampaignsClientConfiguration& config)
{
  AWSClient::SetServiceClientName("ConnectCampaigns");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ConnectCampaignsClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateCampaignOutcome ConnectCampaignsClient::CreateCampaign(const CreateCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns");
  return CreateCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

CreateCampaignOutcomeCallable ConnectCampaignsClient::CreateCampaignCallable(const CreateCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::CreateCampaignAsync(const CreateCampaignRequest& request, const CreateCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateCampaign, request, handler, context, m_executor.get());
}

DeleteCampaignOutcome ConnectCampaignsClient::DeleteCampaign(const DeleteCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCampaign", "Required field: Id, is not set");
    return DeleteCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DeleteCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteCampaignOutcomeCallable ConnectCampaignsClient::DeleteCampaignCallable(const DeleteCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::DeleteCampaignAsync(const DeleteCampaignRequest& request, const DeleteCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteCampaign, request, handler, context, m_executor.get());
}

DeleteConnectInstanceConfigOutcome ConnectCampaignsClient::DeleteConnectInstanceConfig(const DeleteConnectInstanceConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConnectInstanceConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectInstanceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConnectInstanceConfig", "Required field: ConnectInstanceId, is not set");
    return DeleteConnectInstanceConfigOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectInstanceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConnectInstanceConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-instance/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectInstanceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/config");
  return DeleteConnectInstanceConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteConnectInstanceConfigOutcomeCallable ConnectCampaignsClient::DeleteConnectInstanceConfigCallable(const DeleteConnectInstanceConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteConnectInstanceConfig, request, m_executor.get());
}

void ConnectCampaignsClient::DeleteConnectInstanceConfigAsync(const DeleteConnectInstanceConfigRequest& request, const DeleteConnectInstanceConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteConnectInstanceConfig, request, handler, context, m_executor.get());
}

DeleteInstanceOnboardingJobOutcome ConnectCampaignsClient::DeleteInstanceOnboardingJob(const DeleteInstanceOnboardingJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteInstanceOnboardingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectInstanceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteInstanceOnboardingJob", "Required field: ConnectInstanceId, is not set");
    return DeleteInstanceOnboardingJobOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectInstanceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteInstanceOnboardingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-instance/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectInstanceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/onboarding");
  return DeleteInstanceOnboardingJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteInstanceOnboardingJobOutcomeCallable ConnectCampaignsClient::DeleteInstanceOnboardingJobCallable(const DeleteInstanceOnboardingJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteInstanceOnboardingJob, request, m_executor.get());
}

void ConnectCampaignsClient::DeleteInstanceOnboardingJobAsync(const DeleteInstanceOnboardingJobRequest& request, const DeleteInstanceOnboardingJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteInstanceOnboardingJob, request, handler, context, m_executor.get());
}

DescribeCampaignOutcome ConnectCampaignsClient::DescribeCampaign(const DescribeCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeCampaign", "Required field: Id, is not set");
    return DescribeCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DescribeCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeCampaignOutcomeCallable ConnectCampaignsClient::DescribeCampaignCallable(const DescribeCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::DescribeCampaignAsync(const DescribeCampaignRequest& request, const DescribeCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeCampaign, request, handler, context, m_executor.get());
}

GetCampaignStateOutcome ConnectCampaignsClient::GetCampaignState(const GetCampaignStateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCampaignState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCampaignState", "Required field: Id, is not set");
    return GetCampaignStateOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCampaignState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/state");
  return GetCampaignStateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCampaignStateOutcomeCallable ConnectCampaignsClient::GetCampaignStateCallable(const GetCampaignStateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCampaignState, request, m_executor.get());
}

void ConnectCampaignsClient::GetCampaignStateAsync(const GetCampaignStateRequest& request, const GetCampaignStateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCampaignState, request, handler, context, m_executor.get());
}

GetCampaignStateBatchOutcome ConnectCampaignsClient::GetCampaignStateBatch(const GetCampaignStateBatchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCampaignStateBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCampaignStateBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns-state");
  return GetCampaignStateBatchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetCampaignStateBatchOutcomeCallable ConnectCampaignsClient::GetCampaignStateBatchCallable(const GetCampaignStateBatchRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCampaignStateBatch, request, m_executor.get());
}

void ConnectCampaignsClient::GetCampaignStateBatchAsync(const GetCampaignStateBatchRequest& request, const GetCampaignStateBatchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCampaignStateBatch, request, handler, context, m_executor.get());
}

GetConnectInstanceConfigOutcome ConnectCampaignsClient::GetConnectInstanceConfig(const GetConnectInstanceConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConnectInstanceConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectInstanceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConnectInstanceConfig", "Required field: ConnectInstanceId, is not set");
    return GetConnectInstanceConfigOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectInstanceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConnectInstanceConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-instance/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectInstanceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/config");
  return GetConnectInstanceConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConnectInstanceConfigOutcomeCallable ConnectCampaignsClient::GetConnectInstanceConfigCallable(const GetConnectInstanceConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConnectInstanceConfig, request, m_executor.get());
}

void ConnectCampaignsClient::GetConnectInstanceConfigAsync(const GetConnectInstanceConfigRequest& request, const GetConnectInstanceConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConnectInstanceConfig, request, handler, context, m_executor.get());
}

GetInstanceOnboardingJobStatusOutcome ConnectCampaignsClient::GetInstanceOnboardingJobStatus(const GetInstanceOnboardingJobStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetInstanceOnboardingJobStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectInstanceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInstanceOnboardingJobStatus", "Required field: ConnectInstanceId, is not set");
    return GetInstanceOnboardingJobStatusOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectInstanceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetInstanceOnboardingJobStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-instance/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectInstanceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/onboarding");
  return GetInstanceOnboardingJobStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetInstanceOnboardingJobStatusOutcomeCallable ConnectCampaignsClient::GetInstanceOnboardingJobStatusCallable(const GetInstanceOnboardingJobStatusRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetInstanceOnboardingJobStatus, request, m_executor.get());
}

void ConnectCampaignsClient::GetInstanceOnboardingJobStatusAsync(const GetInstanceOnboardingJobStatusRequest& request, const GetInstanceOnboardingJobStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetInstanceOnboardingJobStatus, request, handler, context, m_executor.get());
}

ListCampaignsOutcome ConnectCampaignsClient::ListCampaigns(const ListCampaignsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCampaigns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCampaigns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns-summary");
  return ListCampaignsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListCampaignsOutcomeCallable ConnectCampaignsClient::ListCampaignsCallable(const ListCampaignsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListCampaigns, request, m_executor.get());
}

void ConnectCampaignsClient::ListCampaignsAsync(const ListCampaignsRequest& request, const ListCampaignsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListCampaigns, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome ConnectCampaignsClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: Arn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable ConnectCampaignsClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void ConnectCampaignsClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

PauseCampaignOutcome ConnectCampaignsClient::PauseCampaign(const PauseCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PauseCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PauseCampaign", "Required field: Id, is not set");
    return PauseCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PauseCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/pause");
  return PauseCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PauseCampaignOutcomeCallable ConnectCampaignsClient::PauseCampaignCallable(const PauseCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PauseCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::PauseCampaignAsync(const PauseCampaignRequest& request, const PauseCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PauseCampaign, request, handler, context, m_executor.get());
}

PutDialRequestBatchOutcome ConnectCampaignsClient::PutDialRequestBatch(const PutDialRequestBatchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutDialRequestBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutDialRequestBatch", "Required field: Id, is not set");
    return PutDialRequestBatchOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutDialRequestBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/dial-requests");
  return PutDialRequestBatchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutDialRequestBatchOutcomeCallable ConnectCampaignsClient::PutDialRequestBatchCallable(const PutDialRequestBatchRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutDialRequestBatch, request, m_executor.get());
}

void ConnectCampaignsClient::PutDialRequestBatchAsync(const PutDialRequestBatchRequest& request, const PutDialRequestBatchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutDialRequestBatch, request, handler, context, m_executor.get());
}

ResumeCampaignOutcome ConnectCampaignsClient::ResumeCampaign(const ResumeCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResumeCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ResumeCampaign", "Required field: Id, is not set");
    return ResumeCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResumeCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resume");
  return ResumeCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ResumeCampaignOutcomeCallable ConnectCampaignsClient::ResumeCampaignCallable(const ResumeCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ResumeCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::ResumeCampaignAsync(const ResumeCampaignRequest& request, const ResumeCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ResumeCampaign, request, handler, context, m_executor.get());
}

StartCampaignOutcome ConnectCampaignsClient::StartCampaign(const StartCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartCampaign", "Required field: Id, is not set");
    return StartCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start");
  return StartCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartCampaignOutcomeCallable ConnectCampaignsClient::StartCampaignCallable(const StartCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::StartCampaignAsync(const StartCampaignRequest& request, const StartCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartCampaign, request, handler, context, m_executor.get());
}

StartInstanceOnboardingJobOutcome ConnectCampaignsClient::StartInstanceOnboardingJob(const StartInstanceOnboardingJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartInstanceOnboardingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectInstanceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartInstanceOnboardingJob", "Required field: ConnectInstanceId, is not set");
    return StartInstanceOnboardingJobOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectInstanceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartInstanceOnboardingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-instance/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectInstanceId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/onboarding");
  return StartInstanceOnboardingJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

StartInstanceOnboardingJobOutcomeCallable ConnectCampaignsClient::StartInstanceOnboardingJobCallable(const StartInstanceOnboardingJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartInstanceOnboardingJob, request, m_executor.get());
}

void ConnectCampaignsClient::StartInstanceOnboardingJobAsync(const StartInstanceOnboardingJobRequest& request, const StartInstanceOnboardingJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartInstanceOnboardingJob, request, handler, context, m_executor.get());
}

StopCampaignOutcome ConnectCampaignsClient::StopCampaign(const StopCampaignRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopCampaign", "Required field: Id, is not set");
    return StopCampaignOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopCampaign, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stop");
  return StopCampaignOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopCampaignOutcomeCallable ConnectCampaignsClient::StopCampaignCallable(const StopCampaignRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StopCampaign, request, m_executor.get());
}

void ConnectCampaignsClient::StopCampaignAsync(const StopCampaignRequest& request, const StopCampaignResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StopCampaign, request, handler, context, m_executor.get());
}

TagResourceOutcome ConnectCampaignsClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: Arn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable ConnectCampaignsClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void ConnectCampaignsClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome ConnectCampaignsClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: Arn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Arn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable ConnectCampaignsClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void ConnectCampaignsClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateCampaignDialerConfigOutcome ConnectCampaignsClient::UpdateCampaignDialerConfig(const UpdateCampaignDialerConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCampaignDialerConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateCampaignDialerConfig", "Required field: Id, is not set");
    return UpdateCampaignDialerConfigOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCampaignDialerConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/dialer-config");
  return UpdateCampaignDialerConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateCampaignDialerConfigOutcomeCallable ConnectCampaignsClient::UpdateCampaignDialerConfigCallable(const UpdateCampaignDialerConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCampaignDialerConfig, request, m_executor.get());
}

void ConnectCampaignsClient::UpdateCampaignDialerConfigAsync(const UpdateCampaignDialerConfigRequest& request, const UpdateCampaignDialerConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCampaignDialerConfig, request, handler, context, m_executor.get());
}

UpdateCampaignNameOutcome ConnectCampaignsClient::UpdateCampaignName(const UpdateCampaignNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCampaignName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateCampaignName", "Required field: Id, is not set");
    return UpdateCampaignNameOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCampaignName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/name");
  return UpdateCampaignNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateCampaignNameOutcomeCallable ConnectCampaignsClient::UpdateCampaignNameCallable(const UpdateCampaignNameRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCampaignName, request, m_executor.get());
}

void ConnectCampaignsClient::UpdateCampaignNameAsync(const UpdateCampaignNameRequest& request, const UpdateCampaignNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCampaignName, request, handler, context, m_executor.get());
}

UpdateCampaignOutboundCallConfigOutcome ConnectCampaignsClient::UpdateCampaignOutboundCallConfig(const UpdateCampaignOutboundCallConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCampaignOutboundCallConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateCampaignOutboundCallConfig", "Required field: Id, is not set");
    return UpdateCampaignOutboundCallConfigOutcome(Aws::Client::AWSError<ConnectCampaignsErrors>(ConnectCampaignsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCampaignOutboundCallConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/campaigns/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/outbound-call-config");
  return UpdateCampaignOutboundCallConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateCampaignOutboundCallConfigOutcomeCallable ConnectCampaignsClient::UpdateCampaignOutboundCallConfigCallable(const UpdateCampaignOutboundCallConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCampaignOutboundCallConfig, request, m_executor.get());
}

void ConnectCampaignsClient::UpdateCampaignOutboundCallConfigAsync(const UpdateCampaignOutboundCallConfigRequest& request, const UpdateCampaignOutboundCallConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCampaignOutboundCallConfig, request, handler, context, m_executor.get());
}

