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

#include <aws/resiliencehub/ResilienceHubClient.h>
#include <aws/resiliencehub/ResilienceHubErrorMarshaller.h>
#include <aws/resiliencehub/ResilienceHubEndpointProvider.h>
#include <aws/resiliencehub/model/AddDraftAppVersionResourceMappingsRequest.h>
#include <aws/resiliencehub/model/CreateAppRequest.h>
#include <aws/resiliencehub/model/CreateRecommendationTemplateRequest.h>
#include <aws/resiliencehub/model/CreateResiliencyPolicyRequest.h>
#include <aws/resiliencehub/model/DeleteAppRequest.h>
#include <aws/resiliencehub/model/DeleteAppAssessmentRequest.h>
#include <aws/resiliencehub/model/DeleteRecommendationTemplateRequest.h>
#include <aws/resiliencehub/model/DeleteResiliencyPolicyRequest.h>
#include <aws/resiliencehub/model/DescribeAppRequest.h>
#include <aws/resiliencehub/model/DescribeAppAssessmentRequest.h>
#include <aws/resiliencehub/model/DescribeAppVersionResourcesResolutionStatusRequest.h>
#include <aws/resiliencehub/model/DescribeAppVersionTemplateRequest.h>
#include <aws/resiliencehub/model/DescribeDraftAppVersionResourcesImportStatusRequest.h>
#include <aws/resiliencehub/model/DescribeResiliencyPolicyRequest.h>
#include <aws/resiliencehub/model/ImportResourcesToDraftAppVersionRequest.h>
#include <aws/resiliencehub/model/ListAlarmRecommendationsRequest.h>
#include <aws/resiliencehub/model/ListAppAssessmentsRequest.h>
#include <aws/resiliencehub/model/ListAppComponentCompliancesRequest.h>
#include <aws/resiliencehub/model/ListAppComponentRecommendationsRequest.h>
#include <aws/resiliencehub/model/ListAppVersionResourceMappingsRequest.h>
#include <aws/resiliencehub/model/ListAppVersionResourcesRequest.h>
#include <aws/resiliencehub/model/ListAppVersionsRequest.h>
#include <aws/resiliencehub/model/ListAppsRequest.h>
#include <aws/resiliencehub/model/ListRecommendationTemplatesRequest.h>
#include <aws/resiliencehub/model/ListResiliencyPoliciesRequest.h>
#include <aws/resiliencehub/model/ListSopRecommendationsRequest.h>
#include <aws/resiliencehub/model/ListSuggestedResiliencyPoliciesRequest.h>
#include <aws/resiliencehub/model/ListTagsForResourceRequest.h>
#include <aws/resiliencehub/model/ListTestRecommendationsRequest.h>
#include <aws/resiliencehub/model/ListUnsupportedAppVersionResourcesRequest.h>
#include <aws/resiliencehub/model/PublishAppVersionRequest.h>
#include <aws/resiliencehub/model/PutDraftAppVersionTemplateRequest.h>
#include <aws/resiliencehub/model/RemoveDraftAppVersionResourceMappingsRequest.h>
#include <aws/resiliencehub/model/ResolveAppVersionResourcesRequest.h>
#include <aws/resiliencehub/model/StartAppAssessmentRequest.h>
#include <aws/resiliencehub/model/TagResourceRequest.h>
#include <aws/resiliencehub/model/UntagResourceRequest.h>
#include <aws/resiliencehub/model/UpdateAppRequest.h>
#include <aws/resiliencehub/model/UpdateResiliencyPolicyRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::ResilienceHub;
using namespace Aws::ResilienceHub::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* ResilienceHubClient::SERVICE_NAME = "resiliencehub";
const char* ResilienceHubClient::ALLOCATION_TAG = "ResilienceHubClient";

ResilienceHubClient::ResilienceHubClient(const ResilienceHub::ResilienceHubClientConfiguration& clientConfiguration,
                                         std::shared_ptr<ResilienceHubEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ResilienceHubClient::ResilienceHubClient(const AWSCredentials& credentials,
                                         std::shared_ptr<ResilienceHubEndpointProviderBase> endpointProvider,
                                         const ResilienceHub::ResilienceHubClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

ResilienceHubClient::ResilienceHubClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                         std::shared_ptr<ResilienceHubEndpointProviderBase> endpointProvider,
                                         const ResilienceHub::ResilienceHubClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  ResilienceHubClient::ResilienceHubClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<ResilienceHubEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ResilienceHubClient::ResilienceHubClient(const AWSCredentials& credentials,
                                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ResilienceHubEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

ResilienceHubClient::ResilienceHubClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<ResilienceHubErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<ResilienceHubEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
ResilienceHubClient::~ResilienceHubClient()
{
}

std::shared_ptr<ResilienceHubEndpointProviderBase>& ResilienceHubClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void ResilienceHubClient::init(const ResilienceHub::ResilienceHubClientConfiguration& config)
{
  AWSClient::SetServiceClientName("resiliencehub");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void ResilienceHubClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AddDraftAppVersionResourceMappingsOutcome ResilienceHubClient::AddDraftAppVersionResourceMappings(const AddDraftAppVersionResourceMappingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddDraftAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddDraftAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/add-draft-app-version-resource-mappings");
  return AddDraftAppVersionResourceMappingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AddDraftAppVersionResourceMappingsOutcomeCallable ResilienceHubClient::AddDraftAppVersionResourceMappingsCallable(const AddDraftAppVersionResourceMappingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::AddDraftAppVersionResourceMappings, this, request, m_executor.get());
}

void ResilienceHubClient::AddDraftAppVersionResourceMappingsAsync(const AddDraftAppVersionResourceMappingsRequest& request, const AddDraftAppVersionResourceMappingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::AddDraftAppVersionResourceMappings, this, request, handler, context, m_executor.get());
}

CreateAppOutcome ResilienceHubClient::CreateApp(const CreateAppRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/create-app");
  return CreateAppOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAppOutcomeCallable ResilienceHubClient::CreateAppCallable(const CreateAppRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::CreateApp, this, request, m_executor.get());
}

void ResilienceHubClient::CreateAppAsync(const CreateAppRequest& request, const CreateAppResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::CreateApp, this, request, handler, context, m_executor.get());
}

CreateRecommendationTemplateOutcome ResilienceHubClient::CreateRecommendationTemplate(const CreateRecommendationTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRecommendationTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRecommendationTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/create-recommendation-template");
  return CreateRecommendationTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRecommendationTemplateOutcomeCallable ResilienceHubClient::CreateRecommendationTemplateCallable(const CreateRecommendationTemplateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::CreateRecommendationTemplate, this, request, m_executor.get());
}

void ResilienceHubClient::CreateRecommendationTemplateAsync(const CreateRecommendationTemplateRequest& request, const CreateRecommendationTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::CreateRecommendationTemplate, this, request, handler, context, m_executor.get());
}

CreateResiliencyPolicyOutcome ResilienceHubClient::CreateResiliencyPolicy(const CreateResiliencyPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/create-resiliency-policy");
  return CreateResiliencyPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateResiliencyPolicyOutcomeCallable ResilienceHubClient::CreateResiliencyPolicyCallable(const CreateResiliencyPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::CreateResiliencyPolicy, this, request, m_executor.get());
}

void ResilienceHubClient::CreateResiliencyPolicyAsync(const CreateResiliencyPolicyRequest& request, const CreateResiliencyPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::CreateResiliencyPolicy, this, request, handler, context, m_executor.get());
}

DeleteAppOutcome ResilienceHubClient::DeleteApp(const DeleteAppRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/delete-app");
  return DeleteAppOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteAppOutcomeCallable ResilienceHubClient::DeleteAppCallable(const DeleteAppRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DeleteApp, this, request, m_executor.get());
}

void ResilienceHubClient::DeleteAppAsync(const DeleteAppRequest& request, const DeleteAppResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DeleteApp, this, request, handler, context, m_executor.get());
}

DeleteAppAssessmentOutcome ResilienceHubClient::DeleteAppAssessment(const DeleteAppAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/delete-app-assessment");
  return DeleteAppAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteAppAssessmentOutcomeCallable ResilienceHubClient::DeleteAppAssessmentCallable(const DeleteAppAssessmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DeleteAppAssessment, this, request, m_executor.get());
}

void ResilienceHubClient::DeleteAppAssessmentAsync(const DeleteAppAssessmentRequest& request, const DeleteAppAssessmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DeleteAppAssessment, this, request, handler, context, m_executor.get());
}

DeleteRecommendationTemplateOutcome ResilienceHubClient::DeleteRecommendationTemplate(const DeleteRecommendationTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRecommendationTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRecommendationTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/delete-recommendation-template");
  return DeleteRecommendationTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRecommendationTemplateOutcomeCallable ResilienceHubClient::DeleteRecommendationTemplateCallable(const DeleteRecommendationTemplateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DeleteRecommendationTemplate, this, request, m_executor.get());
}

void ResilienceHubClient::DeleteRecommendationTemplateAsync(const DeleteRecommendationTemplateRequest& request, const DeleteRecommendationTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DeleteRecommendationTemplate, this, request, handler, context, m_executor.get());
}

DeleteResiliencyPolicyOutcome ResilienceHubClient::DeleteResiliencyPolicy(const DeleteResiliencyPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/delete-resiliency-policy");
  return DeleteResiliencyPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteResiliencyPolicyOutcomeCallable ResilienceHubClient::DeleteResiliencyPolicyCallable(const DeleteResiliencyPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DeleteResiliencyPolicy, this, request, m_executor.get());
}

void ResilienceHubClient::DeleteResiliencyPolicyAsync(const DeleteResiliencyPolicyRequest& request, const DeleteResiliencyPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DeleteResiliencyPolicy, this, request, handler, context, m_executor.get());
}

DescribeAppOutcome ResilienceHubClient::DescribeApp(const DescribeAppRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-app");
  return DescribeAppOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAppOutcomeCallable ResilienceHubClient::DescribeAppCallable(const DescribeAppRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeApp, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeAppAsync(const DescribeAppRequest& request, const DescribeAppResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeApp, this, request, handler, context, m_executor.get());
}

DescribeAppAssessmentOutcome ResilienceHubClient::DescribeAppAssessment(const DescribeAppAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-app-assessment");
  return DescribeAppAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAppAssessmentOutcomeCallable ResilienceHubClient::DescribeAppAssessmentCallable(const DescribeAppAssessmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeAppAssessment, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeAppAssessmentAsync(const DescribeAppAssessmentRequest& request, const DescribeAppAssessmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeAppAssessment, this, request, handler, context, m_executor.get());
}

DescribeAppVersionResourcesResolutionStatusOutcome ResilienceHubClient::DescribeAppVersionResourcesResolutionStatus(const DescribeAppVersionResourcesResolutionStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAppVersionResourcesResolutionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAppVersionResourcesResolutionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-app-version-resources-resolution-status");
  return DescribeAppVersionResourcesResolutionStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAppVersionResourcesResolutionStatusOutcomeCallable ResilienceHubClient::DescribeAppVersionResourcesResolutionStatusCallable(const DescribeAppVersionResourcesResolutionStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeAppVersionResourcesResolutionStatus, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeAppVersionResourcesResolutionStatusAsync(const DescribeAppVersionResourcesResolutionStatusRequest& request, const DescribeAppVersionResourcesResolutionStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeAppVersionResourcesResolutionStatus, this, request, handler, context, m_executor.get());
}

DescribeAppVersionTemplateOutcome ResilienceHubClient::DescribeAppVersionTemplate(const DescribeAppVersionTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAppVersionTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAppVersionTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-app-version-template");
  return DescribeAppVersionTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAppVersionTemplateOutcomeCallable ResilienceHubClient::DescribeAppVersionTemplateCallable(const DescribeAppVersionTemplateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeAppVersionTemplate, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeAppVersionTemplateAsync(const DescribeAppVersionTemplateRequest& request, const DescribeAppVersionTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeAppVersionTemplate, this, request, handler, context, m_executor.get());
}

DescribeDraftAppVersionResourcesImportStatusOutcome ResilienceHubClient::DescribeDraftAppVersionResourcesImportStatus(const DescribeDraftAppVersionResourcesImportStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDraftAppVersionResourcesImportStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDraftAppVersionResourcesImportStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-draft-app-version-resources-import-status");
  return DescribeDraftAppVersionResourcesImportStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeDraftAppVersionResourcesImportStatusOutcomeCallable ResilienceHubClient::DescribeDraftAppVersionResourcesImportStatusCallable(const DescribeDraftAppVersionResourcesImportStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeDraftAppVersionResourcesImportStatus, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeDraftAppVersionResourcesImportStatusAsync(const DescribeDraftAppVersionResourcesImportStatusRequest& request, const DescribeDraftAppVersionResourcesImportStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeDraftAppVersionResourcesImportStatus, this, request, handler, context, m_executor.get());
}

DescribeResiliencyPolicyOutcome ResilienceHubClient::DescribeResiliencyPolicy(const DescribeResiliencyPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describe-resiliency-policy");
  return DescribeResiliencyPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeResiliencyPolicyOutcomeCallable ResilienceHubClient::DescribeResiliencyPolicyCallable(const DescribeResiliencyPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::DescribeResiliencyPolicy, this, request, m_executor.get());
}

void ResilienceHubClient::DescribeResiliencyPolicyAsync(const DescribeResiliencyPolicyRequest& request, const DescribeResiliencyPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::DescribeResiliencyPolicy, this, request, handler, context, m_executor.get());
}

ImportResourcesToDraftAppVersionOutcome ResilienceHubClient::ImportResourcesToDraftAppVersion(const ImportResourcesToDraftAppVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportResourcesToDraftAppVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportResourcesToDraftAppVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/import-resources-to-draft-app-version");
  return ImportResourcesToDraftAppVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportResourcesToDraftAppVersionOutcomeCallable ResilienceHubClient::ImportResourcesToDraftAppVersionCallable(const ImportResourcesToDraftAppVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ImportResourcesToDraftAppVersion, this, request, m_executor.get());
}

void ResilienceHubClient::ImportResourcesToDraftAppVersionAsync(const ImportResourcesToDraftAppVersionRequest& request, const ImportResourcesToDraftAppVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ImportResourcesToDraftAppVersion, this, request, handler, context, m_executor.get());
}

ListAlarmRecommendationsOutcome ResilienceHubClient::ListAlarmRecommendations(const ListAlarmRecommendationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAlarmRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAlarmRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-alarm-recommendations");
  return ListAlarmRecommendationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAlarmRecommendationsOutcomeCallable ResilienceHubClient::ListAlarmRecommendationsCallable(const ListAlarmRecommendationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAlarmRecommendations, this, request, m_executor.get());
}

void ResilienceHubClient::ListAlarmRecommendationsAsync(const ListAlarmRecommendationsRequest& request, const ListAlarmRecommendationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAlarmRecommendations, this, request, handler, context, m_executor.get());
}

ListAppAssessmentsOutcome ResilienceHubClient::ListAppAssessments(const ListAppAssessmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppAssessments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppAssessments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-assessments");
  return ListAppAssessmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAppAssessmentsOutcomeCallable ResilienceHubClient::ListAppAssessmentsCallable(const ListAppAssessmentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppAssessments, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppAssessmentsAsync(const ListAppAssessmentsRequest& request, const ListAppAssessmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppAssessments, this, request, handler, context, m_executor.get());
}

ListAppComponentCompliancesOutcome ResilienceHubClient::ListAppComponentCompliances(const ListAppComponentCompliancesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppComponentCompliances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppComponentCompliances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-component-compliances");
  return ListAppComponentCompliancesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppComponentCompliancesOutcomeCallable ResilienceHubClient::ListAppComponentCompliancesCallable(const ListAppComponentCompliancesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppComponentCompliances, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppComponentCompliancesAsync(const ListAppComponentCompliancesRequest& request, const ListAppComponentCompliancesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppComponentCompliances, this, request, handler, context, m_executor.get());
}

ListAppComponentRecommendationsOutcome ResilienceHubClient::ListAppComponentRecommendations(const ListAppComponentRecommendationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppComponentRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppComponentRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-component-recommendations");
  return ListAppComponentRecommendationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppComponentRecommendationsOutcomeCallable ResilienceHubClient::ListAppComponentRecommendationsCallable(const ListAppComponentRecommendationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppComponentRecommendations, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppComponentRecommendationsAsync(const ListAppComponentRecommendationsRequest& request, const ListAppComponentRecommendationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppComponentRecommendations, this, request, handler, context, m_executor.get());
}

ListAppVersionResourceMappingsOutcome ResilienceHubClient::ListAppVersionResourceMappings(const ListAppVersionResourceMappingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-version-resource-mappings");
  return ListAppVersionResourceMappingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppVersionResourceMappingsOutcomeCallable ResilienceHubClient::ListAppVersionResourceMappingsCallable(const ListAppVersionResourceMappingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppVersionResourceMappings, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppVersionResourceMappingsAsync(const ListAppVersionResourceMappingsRequest& request, const ListAppVersionResourceMappingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppVersionResourceMappings, this, request, handler, context, m_executor.get());
}

ListAppVersionResourcesOutcome ResilienceHubClient::ListAppVersionResources(const ListAppVersionResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-version-resources");
  return ListAppVersionResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppVersionResourcesOutcomeCallable ResilienceHubClient::ListAppVersionResourcesCallable(const ListAppVersionResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppVersionResources, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppVersionResourcesAsync(const ListAppVersionResourcesRequest& request, const ListAppVersionResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppVersionResources, this, request, handler, context, m_executor.get());
}

ListAppVersionsOutcome ResilienceHubClient::ListAppVersions(const ListAppVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAppVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAppVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-app-versions");
  return ListAppVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAppVersionsOutcomeCallable ResilienceHubClient::ListAppVersionsCallable(const ListAppVersionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListAppVersions, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppVersionsAsync(const ListAppVersionsRequest& request, const ListAppVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListAppVersions, this, request, handler, context, m_executor.get());
}

ListAppsOutcome ResilienceHubClient::ListApps(const ListAppsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListApps, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListApps, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-apps");
  return ListAppsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAppsOutcomeCallable ResilienceHubClient::ListAppsCallable(const ListAppsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListApps, this, request, m_executor.get());
}

void ResilienceHubClient::ListAppsAsync(const ListAppsRequest& request, const ListAppsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListApps, this, request, handler, context, m_executor.get());
}

ListRecommendationTemplatesOutcome ResilienceHubClient::ListRecommendationTemplates(const ListRecommendationTemplatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRecommendationTemplates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssessmentArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListRecommendationTemplates", "Required field: AssessmentArn, is not set");
    return ListRecommendationTemplatesOutcome(Aws::Client::AWSError<ResilienceHubErrors>(ResilienceHubErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssessmentArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRecommendationTemplates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-recommendation-templates");
  return ListRecommendationTemplatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListRecommendationTemplatesOutcomeCallable ResilienceHubClient::ListRecommendationTemplatesCallable(const ListRecommendationTemplatesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListRecommendationTemplates, this, request, m_executor.get());
}

void ResilienceHubClient::ListRecommendationTemplatesAsync(const ListRecommendationTemplatesRequest& request, const ListRecommendationTemplatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListRecommendationTemplates, this, request, handler, context, m_executor.get());
}

ListResiliencyPoliciesOutcome ResilienceHubClient::ListResiliencyPolicies(const ListResiliencyPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResiliencyPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResiliencyPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-resiliency-policies");
  return ListResiliencyPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListResiliencyPoliciesOutcomeCallable ResilienceHubClient::ListResiliencyPoliciesCallable(const ListResiliencyPoliciesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListResiliencyPolicies, this, request, m_executor.get());
}

void ResilienceHubClient::ListResiliencyPoliciesAsync(const ListResiliencyPoliciesRequest& request, const ListResiliencyPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListResiliencyPolicies, this, request, handler, context, m_executor.get());
}

ListSopRecommendationsOutcome ResilienceHubClient::ListSopRecommendations(const ListSopRecommendationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSopRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSopRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-sop-recommendations");
  return ListSopRecommendationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSopRecommendationsOutcomeCallable ResilienceHubClient::ListSopRecommendationsCallable(const ListSopRecommendationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListSopRecommendations, this, request, m_executor.get());
}

void ResilienceHubClient::ListSopRecommendationsAsync(const ListSopRecommendationsRequest& request, const ListSopRecommendationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListSopRecommendations, this, request, handler, context, m_executor.get());
}

ListSuggestedResiliencyPoliciesOutcome ResilienceHubClient::ListSuggestedResiliencyPolicies(const ListSuggestedResiliencyPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSuggestedResiliencyPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSuggestedResiliencyPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-suggested-resiliency-policies");
  return ListSuggestedResiliencyPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSuggestedResiliencyPoliciesOutcomeCallable ResilienceHubClient::ListSuggestedResiliencyPoliciesCallable(const ListSuggestedResiliencyPoliciesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListSuggestedResiliencyPolicies, this, request, m_executor.get());
}

void ResilienceHubClient::ListSuggestedResiliencyPoliciesAsync(const ListSuggestedResiliencyPoliciesRequest& request, const ListSuggestedResiliencyPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListSuggestedResiliencyPolicies, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome ResilienceHubClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<ResilienceHubErrors>(ResilienceHubErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable ResilienceHubClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListTagsForResource, this, request, m_executor.get());
}

void ResilienceHubClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

ListTestRecommendationsOutcome ResilienceHubClient::ListTestRecommendations(const ListTestRecommendationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTestRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTestRecommendations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-test-recommendations");
  return ListTestRecommendationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTestRecommendationsOutcomeCallable ResilienceHubClient::ListTestRecommendationsCallable(const ListTestRecommendationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListTestRecommendations, this, request, m_executor.get());
}

void ResilienceHubClient::ListTestRecommendationsAsync(const ListTestRecommendationsRequest& request, const ListTestRecommendationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListTestRecommendations, this, request, handler, context, m_executor.get());
}

ListUnsupportedAppVersionResourcesOutcome ResilienceHubClient::ListUnsupportedAppVersionResources(const ListUnsupportedAppVersionResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUnsupportedAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUnsupportedAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/list-unsupported-app-version-resources");
  return ListUnsupportedAppVersionResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUnsupportedAppVersionResourcesOutcomeCallable ResilienceHubClient::ListUnsupportedAppVersionResourcesCallable(const ListUnsupportedAppVersionResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ListUnsupportedAppVersionResources, this, request, m_executor.get());
}

void ResilienceHubClient::ListUnsupportedAppVersionResourcesAsync(const ListUnsupportedAppVersionResourcesRequest& request, const ListUnsupportedAppVersionResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ListUnsupportedAppVersionResources, this, request, handler, context, m_executor.get());
}

PublishAppVersionOutcome ResilienceHubClient::PublishAppVersion(const PublishAppVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PublishAppVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PublishAppVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/publish-app-version");
  return PublishAppVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PublishAppVersionOutcomeCallable ResilienceHubClient::PublishAppVersionCallable(const PublishAppVersionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::PublishAppVersion, this, request, m_executor.get());
}

void ResilienceHubClient::PublishAppVersionAsync(const PublishAppVersionRequest& request, const PublishAppVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::PublishAppVersion, this, request, handler, context, m_executor.get());
}

PutDraftAppVersionTemplateOutcome ResilienceHubClient::PutDraftAppVersionTemplate(const PutDraftAppVersionTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutDraftAppVersionTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutDraftAppVersionTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/put-draft-app-version-template");
  return PutDraftAppVersionTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutDraftAppVersionTemplateOutcomeCallable ResilienceHubClient::PutDraftAppVersionTemplateCallable(const PutDraftAppVersionTemplateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::PutDraftAppVersionTemplate, this, request, m_executor.get());
}

void ResilienceHubClient::PutDraftAppVersionTemplateAsync(const PutDraftAppVersionTemplateRequest& request, const PutDraftAppVersionTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::PutDraftAppVersionTemplate, this, request, handler, context, m_executor.get());
}

RemoveDraftAppVersionResourceMappingsOutcome ResilienceHubClient::RemoveDraftAppVersionResourceMappings(const RemoveDraftAppVersionResourceMappingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RemoveDraftAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RemoveDraftAppVersionResourceMappings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/remove-draft-app-version-resource-mappings");
  return RemoveDraftAppVersionResourceMappingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RemoveDraftAppVersionResourceMappingsOutcomeCallable ResilienceHubClient::RemoveDraftAppVersionResourceMappingsCallable(const RemoveDraftAppVersionResourceMappingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::RemoveDraftAppVersionResourceMappings, this, request, m_executor.get());
}

void ResilienceHubClient::RemoveDraftAppVersionResourceMappingsAsync(const RemoveDraftAppVersionResourceMappingsRequest& request, const RemoveDraftAppVersionResourceMappingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::RemoveDraftAppVersionResourceMappings, this, request, handler, context, m_executor.get());
}

ResolveAppVersionResourcesOutcome ResilienceHubClient::ResolveAppVersionResources(const ResolveAppVersionResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResolveAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResolveAppVersionResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolve-app-version-resources");
  return ResolveAppVersionResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ResolveAppVersionResourcesOutcomeCallable ResilienceHubClient::ResolveAppVersionResourcesCallable(const ResolveAppVersionResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::ResolveAppVersionResources, this, request, m_executor.get());
}

void ResilienceHubClient::ResolveAppVersionResourcesAsync(const ResolveAppVersionResourcesRequest& request, const ResolveAppVersionResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::ResolveAppVersionResources, this, request, handler, context, m_executor.get());
}

StartAppAssessmentOutcome ResilienceHubClient::StartAppAssessment(const StartAppAssessmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartAppAssessment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start-app-assessment");
  return StartAppAssessmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartAppAssessmentOutcomeCallable ResilienceHubClient::StartAppAssessmentCallable(const StartAppAssessmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::StartAppAssessment, this, request, m_executor.get());
}

void ResilienceHubClient::StartAppAssessmentAsync(const StartAppAssessmentRequest& request, const StartAppAssessmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::StartAppAssessment, this, request, handler, context, m_executor.get());
}

TagResourceOutcome ResilienceHubClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<ResilienceHubErrors>(ResilienceHubErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable ResilienceHubClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::TagResource, this, request, m_executor.get());
}

void ResilienceHubClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome ResilienceHubClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<ResilienceHubErrors>(ResilienceHubErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<ResilienceHubErrors>(ResilienceHubErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable ResilienceHubClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::UntagResource, this, request, m_executor.get());
}

void ResilienceHubClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateAppOutcome ResilienceHubClient::UpdateApp(const UpdateAppRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-app");
  return UpdateAppOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateAppOutcomeCallable ResilienceHubClient::UpdateAppCallable(const UpdateAppRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::UpdateApp, this, request, m_executor.get());
}

void ResilienceHubClient::UpdateAppAsync(const UpdateAppRequest& request, const UpdateAppResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::UpdateApp, this, request, handler, context, m_executor.get());
}

UpdateResiliencyPolicyOutcome ResilienceHubClient::UpdateResiliencyPolicy(const UpdateResiliencyPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateResiliencyPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-resiliency-policy");
  return UpdateResiliencyPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateResiliencyPolicyOutcomeCallable ResilienceHubClient::UpdateResiliencyPolicyCallable(const UpdateResiliencyPolicyRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &ResilienceHubClient::UpdateResiliencyPolicy, this, request, m_executor.get());
}

void ResilienceHubClient::UpdateResiliencyPolicyAsync(const UpdateResiliencyPolicyRequest& request, const UpdateResiliencyPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&ResilienceHubClient::UpdateResiliencyPolicy, this, request, handler, context, m_executor.get());
}

