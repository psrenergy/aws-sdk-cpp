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

#include <aws/migrationhubstrategy/MigrationHubStrategyRecommendationsClient.h>
#include <aws/migrationhubstrategy/MigrationHubStrategyRecommendationsErrorMarshaller.h>
#include <aws/migrationhubstrategy/MigrationHubStrategyRecommendationsEndpointProvider.h>
#include <aws/migrationhubstrategy/model/GetApplicationComponentDetailsRequest.h>
#include <aws/migrationhubstrategy/model/GetApplicationComponentStrategiesRequest.h>
#include <aws/migrationhubstrategy/model/GetAssessmentRequest.h>
#include <aws/migrationhubstrategy/model/GetImportFileTaskRequest.h>
#include <aws/migrationhubstrategy/model/GetPortfolioPreferencesRequest.h>
#include <aws/migrationhubstrategy/model/GetPortfolioSummaryRequest.h>
#include <aws/migrationhubstrategy/model/GetRecommendationReportDetailsRequest.h>
#include <aws/migrationhubstrategy/model/GetServerDetailsRequest.h>
#include <aws/migrationhubstrategy/model/GetServerStrategiesRequest.h>
#include <aws/migrationhubstrategy/model/ListApplicationComponentsRequest.h>
#include <aws/migrationhubstrategy/model/ListCollectorsRequest.h>
#include <aws/migrationhubstrategy/model/ListImportFileTaskRequest.h>
#include <aws/migrationhubstrategy/model/ListServersRequest.h>
#include <aws/migrationhubstrategy/model/PutPortfolioPreferencesRequest.h>
#include <aws/migrationhubstrategy/model/StartAssessmentRequest.h>
#include <aws/migrationhubstrategy/model/StartImportFileTaskRequest.h>
#include <aws/migrationhubstrategy/model/StartRecommendationReportGenerationRequest.h>
#include <aws/migrationhubstrategy/model/StopAssessmentRequest.h>
#include <aws/migrationhubstrategy/model/UpdateApplicationComponentConfigRequest.h>
#include <aws/migrationhubstrategy/model/UpdateServerConfigRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::MigrationHubStrategyRecommendations;
using namespace Aws::MigrationHubStrategyRecommendations::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* MigrationHubStrategyRecommendationsClient::SERVICE_NAME = "migrationhub-strategy";
const char* MigrationHubStrategyRecommendationsClient::ALLOCATION_TAG = "MigrationHubStrategyRecommendationsClient";

MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const MigrationHubStrategyRecommendations::MigrationHubStrategyRecommendationsClientConfiguration& clientConfiguration,
                                                                                     std::shared_ptr<MigrationHubStrategyRecommendationsEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const AWSCredentials& credentials,
                                                                                     std::shared_ptr<MigrationHubStrategyRecommendationsEndpointProviderBase> endpointProvider,
                                                                                     const MigrationHubStrategyRecommendations::MigrationHubStrategyRecommendationsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                                                     std::shared_ptr<MigrationHubStrategyRecommendationsEndpointProviderBase> endpointProvider,
                                                                                     const MigrationHubStrategyRecommendations::MigrationHubStrategyRecommendationsClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<MigrationHubStrategyRecommendationsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const AWSCredentials& credentials,
                                                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MigrationHubStrategyRecommendationsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MigrationHubStrategyRecommendationsClient::MigrationHubStrategyRecommendationsClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MigrationHubStrategyRecommendationsErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MigrationHubStrategyRecommendationsEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
MigrationHubStrategyRecommendationsClient::~MigrationHubStrategyRecommendationsClient()
{
}

std::shared_ptr<MigrationHubStrategyRecommendationsEndpointProviderBase>& MigrationHubStrategyRecommendationsClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void MigrationHubStrategyRecommendationsClient::init(const MigrationHubStrategyRecommendations::MigrationHubStrategyRecommendationsClientConfiguration& config)
{
  AWSClient::SetServiceClientName("MigrationHubStrategy");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void MigrationHubStrategyRecommendationsClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

GetApplicationComponentDetailsOutcome MigrationHubStrategyRecommendationsClient::GetApplicationComponentDetails(const GetApplicationComponentDetailsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApplicationComponentDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationComponentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApplicationComponentDetails", "Required field: ApplicationComponentId, is not set");
    return GetApplicationComponentDetailsOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationComponentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApplicationComponentDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-applicationcomponent-details/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationComponentId());
  return GetApplicationComponentDetailsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetApplicationComponentStrategiesOutcome MigrationHubStrategyRecommendationsClient::GetApplicationComponentStrategies(const GetApplicationComponentStrategiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApplicationComponentStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApplicationComponentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApplicationComponentStrategies", "Required field: ApplicationComponentId, is not set");
    return GetApplicationComponentStrategiesOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApplicationComponentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApplicationComponentStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-applicationcomponent-strategies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApplicationComponentId());
  return GetApplicationComponentStrategiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetAssessmentOutcome MigrationHubStrategyRecommendationsClient::GetAssessment(const GetAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAssessment", "Required field: Id, is not set");
    return GetAssessmentOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-assessment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetImportFileTaskOutcome MigrationHubStrategyRecommendationsClient::GetImportFileTask(const GetImportFileTaskRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetImportFileTask", "Required field: Id, is not set");
    return GetImportFileTaskOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-import-file-task/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetImportFileTaskOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetPortfolioPreferencesOutcome MigrationHubStrategyRecommendationsClient::GetPortfolioPreferences(const GetPortfolioPreferencesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPortfolioPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPortfolioPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-portfolio-preferences");
  return GetPortfolioPreferencesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetPortfolioSummaryOutcome MigrationHubStrategyRecommendationsClient::GetPortfolioSummary(const GetPortfolioSummaryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPortfolioSummary, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPortfolioSummary, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-portfolio-summary");
  return GetPortfolioSummaryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetRecommendationReportDetailsOutcome MigrationHubStrategyRecommendationsClient::GetRecommendationReportDetails(const GetRecommendationReportDetailsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRecommendationReportDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRecommendationReportDetails", "Required field: Id, is not set");
    return GetRecommendationReportDetailsOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRecommendationReportDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-recommendation-report-details/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetRecommendationReportDetailsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetServerDetailsOutcome MigrationHubStrategyRecommendationsClient::GetServerDetails(const GetServerDetailsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetServerDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ServerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetServerDetails", "Required field: ServerId, is not set");
    return GetServerDetailsOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ServerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetServerDetails, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-server-details/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetServerId());
  return GetServerDetailsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




GetServerStrategiesOutcome MigrationHubStrategyRecommendationsClient::GetServerStrategies(const GetServerStrategiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetServerStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ServerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetServerStrategies", "Required field: ServerId, is not set");
    return GetServerStrategiesOutcome(Aws::Client::AWSError<MigrationHubStrategyRecommendationsErrors>(MigrationHubStrategyRecommendationsErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ServerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetServerStrategies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/get-server-strategies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetServerId());
  return GetServerStrategiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




ListApplicationComponentsOutcome MigrationHubStrategyRecommendationsClient::ListApplicationComponents(const ListApplicationComponentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListApplicationComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListApplicationComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-applicationcomponents");
  return ListApplicationComponentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




ListCollectorsOutcome MigrationHubStrategyRecommendationsClient::ListCollectors(const ListCollectorsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCollectors, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCollectors, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-collectors");
  return ListCollectorsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




ListImportFileTaskOutcome MigrationHubStrategyRecommendationsClient::ListImportFileTask(const ListImportFileTaskRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-import-file-task");
  return ListImportFileTaskOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}




ListServersOutcome MigrationHubStrategyRecommendationsClient::ListServers(const ListServersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-servers");
  return ListServersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




PutPortfolioPreferencesOutcome MigrationHubStrategyRecommendationsClient::PutPortfolioPreferences(const PutPortfolioPreferencesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutPortfolioPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutPortfolioPreferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/put-portfolio-preferences");
  return PutPortfolioPreferencesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




StartAssessmentOutcome MigrationHubStrategyRecommendationsClient::StartAssessment(const StartAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start-assessment");
  return StartAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




StartImportFileTaskOutcome MigrationHubStrategyRecommendationsClient::StartImportFileTask(const StartImportFileTaskRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartImportFileTask, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start-import-file-task");
  return StartImportFileTaskOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




StartRecommendationReportGenerationOutcome MigrationHubStrategyRecommendationsClient::StartRecommendationReportGeneration(const StartRecommendationReportGenerationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartRecommendationReportGeneration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartRecommendationReportGeneration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start-recommendation-report-generation");
  return StartRecommendationReportGenerationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




StopAssessmentOutcome MigrationHubStrategyRecommendationsClient::StopAssessment(const StopAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stop-assessment");
  return StopAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




UpdateApplicationComponentConfigOutcome MigrationHubStrategyRecommendationsClient::UpdateApplicationComponentConfig(const UpdateApplicationComponentConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApplicationComponentConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApplicationComponentConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-applicationcomponent-config/");
  return UpdateApplicationComponentConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




UpdateServerConfigOutcome MigrationHubStrategyRecommendationsClient::UpdateServerConfig(const UpdateServerConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateServerConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateServerConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-server-config/");
  return UpdateServerConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}




