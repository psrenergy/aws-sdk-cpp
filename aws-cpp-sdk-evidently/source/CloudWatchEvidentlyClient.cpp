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

#include <aws/evidently/CloudWatchEvidentlyClient.h>
#include <aws/evidently/CloudWatchEvidentlyErrorMarshaller.h>
#include <aws/evidently/CloudWatchEvidentlyEndpointProvider.h>
#include <aws/evidently/model/BatchEvaluateFeatureRequest.h>
#include <aws/evidently/model/CreateExperimentRequest.h>
#include <aws/evidently/model/CreateFeatureRequest.h>
#include <aws/evidently/model/CreateLaunchRequest.h>
#include <aws/evidently/model/CreateProjectRequest.h>
#include <aws/evidently/model/CreateSegmentRequest.h>
#include <aws/evidently/model/DeleteExperimentRequest.h>
#include <aws/evidently/model/DeleteFeatureRequest.h>
#include <aws/evidently/model/DeleteLaunchRequest.h>
#include <aws/evidently/model/DeleteProjectRequest.h>
#include <aws/evidently/model/DeleteSegmentRequest.h>
#include <aws/evidently/model/EvaluateFeatureRequest.h>
#include <aws/evidently/model/GetExperimentRequest.h>
#include <aws/evidently/model/GetExperimentResultsRequest.h>
#include <aws/evidently/model/GetFeatureRequest.h>
#include <aws/evidently/model/GetLaunchRequest.h>
#include <aws/evidently/model/GetProjectRequest.h>
#include <aws/evidently/model/GetSegmentRequest.h>
#include <aws/evidently/model/ListExperimentsRequest.h>
#include <aws/evidently/model/ListFeaturesRequest.h>
#include <aws/evidently/model/ListLaunchesRequest.h>
#include <aws/evidently/model/ListProjectsRequest.h>
#include <aws/evidently/model/ListSegmentReferencesRequest.h>
#include <aws/evidently/model/ListSegmentsRequest.h>
#include <aws/evidently/model/ListTagsForResourceRequest.h>
#include <aws/evidently/model/PutProjectEventsRequest.h>
#include <aws/evidently/model/StartExperimentRequest.h>
#include <aws/evidently/model/StartLaunchRequest.h>
#include <aws/evidently/model/StopExperimentRequest.h>
#include <aws/evidently/model/StopLaunchRequest.h>
#include <aws/evidently/model/TagResourceRequest.h>
#include <aws/evidently/model/TestSegmentPatternRequest.h>
#include <aws/evidently/model/UntagResourceRequest.h>
#include <aws/evidently/model/UpdateExperimentRequest.h>
#include <aws/evidently/model/UpdateFeatureRequest.h>
#include <aws/evidently/model/UpdateLaunchRequest.h>
#include <aws/evidently/model/UpdateProjectRequest.h>
#include <aws/evidently/model/UpdateProjectDataDeliveryRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::CloudWatchEvidently;
using namespace Aws::CloudWatchEvidently::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* CloudWatchEvidentlyClient::SERVICE_NAME = "evidently";
const char* CloudWatchEvidentlyClient::ALLOCATION_TAG = "CloudWatchEvidentlyClient";

CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration,
                                                     std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const AWSCredentials& credentials,
                                                     std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider,
                                                     const CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                     std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase> endpointProvider,
                                                     const CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const AWSCredentials& credentials,
                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudWatchEvidentlyClient::CloudWatchEvidentlyClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudWatchEvidentlyErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudWatchEvidentlyEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
CloudWatchEvidentlyClient::~CloudWatchEvidentlyClient()
{
}

std::shared_ptr<CloudWatchEvidentlyEndpointProviderBase>& CloudWatchEvidentlyClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void CloudWatchEvidentlyClient::init(const CloudWatchEvidently::CloudWatchEvidentlyClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Evidently");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void CloudWatchEvidentlyClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchEvaluateFeatureOutcome CloudWatchEvidentlyClient::BatchEvaluateFeature(const BatchEvaluateFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchEvaluateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchEvaluateFeature", "Required field: Project, is not set");
    return BatchEvaluateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchEvaluateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("dataplane.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchEvaluateFeatureOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/evaluations");
  return BatchEvaluateFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchEvaluateFeatureOutcomeCallable CloudWatchEvidentlyClient::BatchEvaluateFeatureCallable(const BatchEvaluateFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::BatchEvaluateFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::BatchEvaluateFeatureAsync(const BatchEvaluateFeatureRequest& request, const BatchEvaluateFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::BatchEvaluateFeature, this, request, handler, context, m_executor.get());
}

CreateExperimentOutcome CloudWatchEvidentlyClient::CreateExperiment(const CreateExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateExperiment", "Required field: Project, is not set");
    return CreateExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments");
  return CreateExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateExperimentOutcomeCallable CloudWatchEvidentlyClient::CreateExperimentCallable(const CreateExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::CreateExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::CreateExperimentAsync(const CreateExperimentRequest& request, const CreateExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::CreateExperiment, this, request, handler, context, m_executor.get());
}

CreateFeatureOutcome CloudWatchEvidentlyClient::CreateFeature(const CreateFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateFeature", "Required field: Project, is not set");
    return CreateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/features");
  return CreateFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateFeatureOutcomeCallable CloudWatchEvidentlyClient::CreateFeatureCallable(const CreateFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::CreateFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::CreateFeatureAsync(const CreateFeatureRequest& request, const CreateFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::CreateFeature, this, request, handler, context, m_executor.get());
}

CreateLaunchOutcome CloudWatchEvidentlyClient::CreateLaunch(const CreateLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateLaunch", "Required field: Project, is not set");
    return CreateLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches");
  return CreateLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateLaunchOutcomeCallable CloudWatchEvidentlyClient::CreateLaunchCallable(const CreateLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::CreateLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::CreateLaunchAsync(const CreateLaunchRequest& request, const CreateLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::CreateLaunch, this, request, handler, context, m_executor.get());
}

CreateProjectOutcome CloudWatchEvidentlyClient::CreateProject(const CreateProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects");
  return CreateProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateProjectOutcomeCallable CloudWatchEvidentlyClient::CreateProjectCallable(const CreateProjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::CreateProject, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::CreateProjectAsync(const CreateProjectRequest& request, const CreateProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::CreateProject, this, request, handler, context, m_executor.get());
}

CreateSegmentOutcome CloudWatchEvidentlyClient::CreateSegment(const CreateSegmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/segments");
  return CreateSegmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSegmentOutcomeCallable CloudWatchEvidentlyClient::CreateSegmentCallable(const CreateSegmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::CreateSegment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::CreateSegmentAsync(const CreateSegmentRequest& request, const CreateSegmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::CreateSegment, this, request, handler, context, m_executor.get());
}

DeleteExperimentOutcome CloudWatchEvidentlyClient::DeleteExperiment(const DeleteExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteExperiment", "Required field: Experiment, is not set");
    return DeleteExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteExperiment", "Required field: Project, is not set");
    return DeleteExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  return DeleteExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteExperimentOutcomeCallable CloudWatchEvidentlyClient::DeleteExperimentCallable(const DeleteExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::DeleteExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::DeleteExperimentAsync(const DeleteExperimentRequest& request, const DeleteExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::DeleteExperiment, this, request, handler, context, m_executor.get());
}

DeleteFeatureOutcome CloudWatchEvidentlyClient::DeleteFeature(const DeleteFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.FeatureHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFeature", "Required field: Feature, is not set");
    return DeleteFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Feature]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFeature", "Required field: Project, is not set");
    return DeleteFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/features/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFeature());
  return DeleteFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteFeatureOutcomeCallable CloudWatchEvidentlyClient::DeleteFeatureCallable(const DeleteFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::DeleteFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::DeleteFeatureAsync(const DeleteFeatureRequest& request, const DeleteFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::DeleteFeature, this, request, handler, context, m_executor.get());
}

DeleteLaunchOutcome CloudWatchEvidentlyClient::DeleteLaunch(const DeleteLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.LaunchHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteLaunch", "Required field: Launch, is not set");
    return DeleteLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Launch]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteLaunch", "Required field: Project, is not set");
    return DeleteLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLaunch());
  return DeleteLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteLaunchOutcomeCallable CloudWatchEvidentlyClient::DeleteLaunchCallable(const DeleteLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::DeleteLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::DeleteLaunchAsync(const DeleteLaunchRequest& request, const DeleteLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::DeleteLaunch, this, request, handler, context, m_executor.get());
}

DeleteProjectOutcome CloudWatchEvidentlyClient::DeleteProject(const DeleteProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteProject", "Required field: Project, is not set");
    return DeleteProjectOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  return DeleteProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteProjectOutcomeCallable CloudWatchEvidentlyClient::DeleteProjectCallable(const DeleteProjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::DeleteProject, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::DeleteProjectAsync(const DeleteProjectRequest& request, const DeleteProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::DeleteProject, this, request, handler, context, m_executor.get());
}

DeleteSegmentOutcome CloudWatchEvidentlyClient::DeleteSegment(const DeleteSegmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SegmentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSegment", "Required field: Segment, is not set");
    return DeleteSegmentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Segment]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/segments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSegment());
  return DeleteSegmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteSegmentOutcomeCallable CloudWatchEvidentlyClient::DeleteSegmentCallable(const DeleteSegmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::DeleteSegment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::DeleteSegmentAsync(const DeleteSegmentRequest& request, const DeleteSegmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::DeleteSegment, this, request, handler, context, m_executor.get());
}

EvaluateFeatureOutcome CloudWatchEvidentlyClient::EvaluateFeature(const EvaluateFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EvaluateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.FeatureHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("EvaluateFeature", "Required field: Feature, is not set");
    return EvaluateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Feature]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("EvaluateFeature", "Required field: Project, is not set");
    return EvaluateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EvaluateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("dataplane.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), EvaluateFeatureOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/evaluations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFeature());
  return EvaluateFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

EvaluateFeatureOutcomeCallable CloudWatchEvidentlyClient::EvaluateFeatureCallable(const EvaluateFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::EvaluateFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::EvaluateFeatureAsync(const EvaluateFeatureRequest& request, const EvaluateFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::EvaluateFeature, this, request, handler, context, m_executor.get());
}

GetExperimentOutcome CloudWatchEvidentlyClient::GetExperiment(const GetExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExperiment", "Required field: Experiment, is not set");
    return GetExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExperiment", "Required field: Project, is not set");
    return GetExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  return GetExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetExperimentOutcomeCallable CloudWatchEvidentlyClient::GetExperimentCallable(const GetExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetExperimentAsync(const GetExperimentRequest& request, const GetExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetExperiment, this, request, handler, context, m_executor.get());
}

GetExperimentResultsOutcome CloudWatchEvidentlyClient::GetExperimentResults(const GetExperimentResultsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetExperimentResults, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExperimentResults", "Required field: Experiment, is not set");
    return GetExperimentResultsOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetExperimentResults", "Required field: Project, is not set");
    return GetExperimentResultsOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetExperimentResults, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  endpointResolutionOutcome.GetResult().AddPathSegments("/results");
  return GetExperimentResultsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetExperimentResultsOutcomeCallable CloudWatchEvidentlyClient::GetExperimentResultsCallable(const GetExperimentResultsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetExperimentResults, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetExperimentResultsAsync(const GetExperimentResultsRequest& request, const GetExperimentResultsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetExperimentResults, this, request, handler, context, m_executor.get());
}

GetFeatureOutcome CloudWatchEvidentlyClient::GetFeature(const GetFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.FeatureHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetFeature", "Required field: Feature, is not set");
    return GetFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Feature]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetFeature", "Required field: Project, is not set");
    return GetFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/features/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFeature());
  return GetFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetFeatureOutcomeCallable CloudWatchEvidentlyClient::GetFeatureCallable(const GetFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetFeatureAsync(const GetFeatureRequest& request, const GetFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetFeature, this, request, handler, context, m_executor.get());
}

GetLaunchOutcome CloudWatchEvidentlyClient::GetLaunch(const GetLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.LaunchHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetLaunch", "Required field: Launch, is not set");
    return GetLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Launch]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetLaunch", "Required field: Project, is not set");
    return GetLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLaunch());
  return GetLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetLaunchOutcomeCallable CloudWatchEvidentlyClient::GetLaunchCallable(const GetLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetLaunchAsync(const GetLaunchRequest& request, const GetLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetLaunch, this, request, handler, context, m_executor.get());
}

GetProjectOutcome CloudWatchEvidentlyClient::GetProject(const GetProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetProject", "Required field: Project, is not set");
    return GetProjectOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  return GetProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetProjectOutcomeCallable CloudWatchEvidentlyClient::GetProjectCallable(const GetProjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetProject, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetProjectAsync(const GetProjectRequest& request, const GetProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetProject, this, request, handler, context, m_executor.get());
}

GetSegmentOutcome CloudWatchEvidentlyClient::GetSegment(const GetSegmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SegmentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSegment", "Required field: Segment, is not set");
    return GetSegmentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Segment]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSegment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/segments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSegment());
  return GetSegmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSegmentOutcomeCallable CloudWatchEvidentlyClient::GetSegmentCallable(const GetSegmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::GetSegment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::GetSegmentAsync(const GetSegmentRequest& request, const GetSegmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::GetSegment, this, request, handler, context, m_executor.get());
}

ListExperimentsOutcome CloudWatchEvidentlyClient::ListExperiments(const ListExperimentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListExperiments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListExperiments", "Required field: Project, is not set");
    return ListExperimentsOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListExperiments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments");
  return ListExperimentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListExperimentsOutcomeCallable CloudWatchEvidentlyClient::ListExperimentsCallable(const ListExperimentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListExperiments, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListExperimentsAsync(const ListExperimentsRequest& request, const ListExperimentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListExperiments, this, request, handler, context, m_executor.get());
}

ListFeaturesOutcome CloudWatchEvidentlyClient::ListFeatures(const ListFeaturesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListFeatures, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListFeatures", "Required field: Project, is not set");
    return ListFeaturesOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListFeatures, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/features");
  return ListFeaturesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListFeaturesOutcomeCallable CloudWatchEvidentlyClient::ListFeaturesCallable(const ListFeaturesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListFeatures, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListFeaturesAsync(const ListFeaturesRequest& request, const ListFeaturesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListFeatures, this, request, handler, context, m_executor.get());
}

ListLaunchesOutcome CloudWatchEvidentlyClient::ListLaunches(const ListLaunchesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListLaunches, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListLaunches", "Required field: Project, is not set");
    return ListLaunchesOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListLaunches, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches");
  return ListLaunchesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListLaunchesOutcomeCallable CloudWatchEvidentlyClient::ListLaunchesCallable(const ListLaunchesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListLaunches, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListLaunchesAsync(const ListLaunchesRequest& request, const ListLaunchesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListLaunches, this, request, handler, context, m_executor.get());
}

ListProjectsOutcome CloudWatchEvidentlyClient::ListProjects(const ListProjectsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects");
  return ListProjectsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListProjectsOutcomeCallable CloudWatchEvidentlyClient::ListProjectsCallable(const ListProjectsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListProjects, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListProjectsAsync(const ListProjectsRequest& request, const ListProjectsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListProjects, this, request, handler, context, m_executor.get());
}

ListSegmentReferencesOutcome CloudWatchEvidentlyClient::ListSegmentReferences(const ListSegmentReferencesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSegmentReferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SegmentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSegmentReferences", "Required field: Segment, is not set");
    return ListSegmentReferencesOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Segment]", false));
  }
  if (!request.TypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSegmentReferences", "Required field: Type, is not set");
    return ListSegmentReferencesOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Type]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSegmentReferences, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/segments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSegment());
  endpointResolutionOutcome.GetResult().AddPathSegments("/references");
  return ListSegmentReferencesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSegmentReferencesOutcomeCallable CloudWatchEvidentlyClient::ListSegmentReferencesCallable(const ListSegmentReferencesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListSegmentReferences, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListSegmentReferencesAsync(const ListSegmentReferencesRequest& request, const ListSegmentReferencesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListSegmentReferences, this, request, handler, context, m_executor.get());
}

ListSegmentsOutcome CloudWatchEvidentlyClient::ListSegments(const ListSegmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSegments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSegments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/segments");
  return ListSegmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSegmentsOutcomeCallable CloudWatchEvidentlyClient::ListSegmentsCallable(const ListSegmentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListSegments, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListSegmentsAsync(const ListSegmentsRequest& request, const ListSegmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListSegments, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome CloudWatchEvidentlyClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable CloudWatchEvidentlyClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::ListTagsForResource, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

PutProjectEventsOutcome CloudWatchEvidentlyClient::PutProjectEvents(const PutProjectEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutProjectEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutProjectEvents", "Required field: Project, is not set");
    return PutProjectEventsOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutProjectEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("dataplane.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), PutProjectEventsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/events/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  return PutProjectEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutProjectEventsOutcomeCallable CloudWatchEvidentlyClient::PutProjectEventsCallable(const PutProjectEventsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::PutProjectEvents, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::PutProjectEventsAsync(const PutProjectEventsRequest& request, const PutProjectEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::PutProjectEvents, this, request, handler, context, m_executor.get());
}

StartExperimentOutcome CloudWatchEvidentlyClient::StartExperiment(const StartExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartExperiment", "Required field: Experiment, is not set");
    return StartExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartExperiment", "Required field: Project, is not set");
    return StartExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start");
  return StartExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartExperimentOutcomeCallable CloudWatchEvidentlyClient::StartExperimentCallable(const StartExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::StartExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::StartExperimentAsync(const StartExperimentRequest& request, const StartExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::StartExperiment, this, request, handler, context, m_executor.get());
}

StartLaunchOutcome CloudWatchEvidentlyClient::StartLaunch(const StartLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.LaunchHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartLaunch", "Required field: Launch, is not set");
    return StartLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Launch]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartLaunch", "Required field: Project, is not set");
    return StartLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLaunch());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start");
  return StartLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartLaunchOutcomeCallable CloudWatchEvidentlyClient::StartLaunchCallable(const StartLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::StartLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::StartLaunchAsync(const StartLaunchRequest& request, const StartLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::StartLaunch, this, request, handler, context, m_executor.get());
}

StopExperimentOutcome CloudWatchEvidentlyClient::StopExperiment(const StopExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopExperiment", "Required field: Experiment, is not set");
    return StopExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopExperiment", "Required field: Project, is not set");
    return StopExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancel");
  return StopExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopExperimentOutcomeCallable CloudWatchEvidentlyClient::StopExperimentCallable(const StopExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::StopExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::StopExperimentAsync(const StopExperimentRequest& request, const StopExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::StopExperiment, this, request, handler, context, m_executor.get());
}

StopLaunchOutcome CloudWatchEvidentlyClient::StopLaunch(const StopLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.LaunchHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopLaunch", "Required field: Launch, is not set");
    return StopLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Launch]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopLaunch", "Required field: Project, is not set");
    return StopLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLaunch());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancel");
  return StopLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopLaunchOutcomeCallable CloudWatchEvidentlyClient::StopLaunchCallable(const StopLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::StopLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::StopLaunchAsync(const StopLaunchRequest& request, const StopLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::StopLaunch, this, request, handler, context, m_executor.get());
}

TagResourceOutcome CloudWatchEvidentlyClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable CloudWatchEvidentlyClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::TagResource, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::TagResource, this, request, handler, context, m_executor.get());
}

TestSegmentPatternOutcome CloudWatchEvidentlyClient::TestSegmentPattern(const TestSegmentPatternRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TestSegmentPattern, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TestSegmentPattern, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/test-segment-pattern");
  return TestSegmentPatternOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TestSegmentPatternOutcomeCallable CloudWatchEvidentlyClient::TestSegmentPatternCallable(const TestSegmentPatternRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::TestSegmentPattern, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::TestSegmentPatternAsync(const TestSegmentPatternRequest& request, const TestSegmentPatternResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::TestSegmentPattern, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome CloudWatchEvidentlyClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable CloudWatchEvidentlyClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UntagResource, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateExperimentOutcome CloudWatchEvidentlyClient::UpdateExperiment(const UpdateExperimentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ExperimentHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateExperiment", "Required field: Experiment, is not set");
    return UpdateExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Experiment]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateExperiment", "Required field: Project, is not set");
    return UpdateExperimentOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateExperiment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/experiments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetExperiment());
  return UpdateExperimentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateExperimentOutcomeCallable CloudWatchEvidentlyClient::UpdateExperimentCallable(const UpdateExperimentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UpdateExperiment, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UpdateExperimentAsync(const UpdateExperimentRequest& request, const UpdateExperimentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UpdateExperiment, this, request, handler, context, m_executor.get());
}

UpdateFeatureOutcome CloudWatchEvidentlyClient::UpdateFeature(const UpdateFeatureRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.FeatureHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateFeature", "Required field: Feature, is not set");
    return UpdateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Feature]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateFeature", "Required field: Project, is not set");
    return UpdateFeatureOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateFeature, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/features/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFeature());
  return UpdateFeatureOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateFeatureOutcomeCallable CloudWatchEvidentlyClient::UpdateFeatureCallable(const UpdateFeatureRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UpdateFeature, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UpdateFeatureAsync(const UpdateFeatureRequest& request, const UpdateFeatureResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UpdateFeature, this, request, handler, context, m_executor.get());
}

UpdateLaunchOutcome CloudWatchEvidentlyClient::UpdateLaunch(const UpdateLaunchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.LaunchHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateLaunch", "Required field: Launch, is not set");
    return UpdateLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Launch]", false));
  }
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateLaunch", "Required field: Project, is not set");
    return UpdateLaunchOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateLaunch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/launches/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLaunch());
  return UpdateLaunchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateLaunchOutcomeCallable CloudWatchEvidentlyClient::UpdateLaunchCallable(const UpdateLaunchRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UpdateLaunch, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UpdateLaunchAsync(const UpdateLaunchRequest& request, const UpdateLaunchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UpdateLaunch, this, request, handler, context, m_executor.get());
}

UpdateProjectOutcome CloudWatchEvidentlyClient::UpdateProject(const UpdateProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateProject", "Required field: Project, is not set");
    return UpdateProjectOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  return UpdateProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateProjectOutcomeCallable CloudWatchEvidentlyClient::UpdateProjectCallable(const UpdateProjectRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UpdateProject, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UpdateProjectAsync(const UpdateProjectRequest& request, const UpdateProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UpdateProject, this, request, handler, context, m_executor.get());
}

UpdateProjectDataDeliveryOutcome CloudWatchEvidentlyClient::UpdateProjectDataDelivery(const UpdateProjectDataDeliveryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateProjectDataDelivery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateProjectDataDelivery", "Required field: Project, is not set");
    return UpdateProjectDataDeliveryOutcome(Aws::Client::AWSError<CloudWatchEvidentlyErrors>(CloudWatchEvidentlyErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Project]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateProjectDataDelivery, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProject());
  endpointResolutionOutcome.GetResult().AddPathSegments("/data-delivery");
  return UpdateProjectDataDeliveryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateProjectDataDeliveryOutcomeCallable CloudWatchEvidentlyClient::UpdateProjectDataDeliveryCallable(const UpdateProjectDataDeliveryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudWatchEvidentlyClient::UpdateProjectDataDelivery, this, request, m_executor.get());
}

void CloudWatchEvidentlyClient::UpdateProjectDataDeliveryAsync(const UpdateProjectDataDeliveryRequest& request, const UpdateProjectDataDeliveryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudWatchEvidentlyClient::UpdateProjectDataDelivery, this, request, handler, context, m_executor.get());
}

