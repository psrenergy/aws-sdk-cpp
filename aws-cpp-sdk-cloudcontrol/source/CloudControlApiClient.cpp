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

#include <aws/cloudcontrol/CloudControlApiClient.h>
#include <aws/cloudcontrol/CloudControlApiErrorMarshaller.h>
#include <aws/cloudcontrol/CloudControlApiEndpointProvider.h>
#include <aws/cloudcontrol/model/CancelResourceRequestRequest.h>
#include <aws/cloudcontrol/model/CreateResourceRequest.h>
#include <aws/cloudcontrol/model/DeleteResourceRequest.h>
#include <aws/cloudcontrol/model/GetResourceRequest.h>
#include <aws/cloudcontrol/model/GetResourceRequestStatusRequest.h>
#include <aws/cloudcontrol/model/ListResourceRequestsRequest.h>
#include <aws/cloudcontrol/model/ListResourcesRequest.h>
#include <aws/cloudcontrol/model/UpdateResourceRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::CloudControlApi;
using namespace Aws::CloudControlApi::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* CloudControlApiClient::SERVICE_NAME = "cloudcontrolapi";
const char* CloudControlApiClient::ALLOCATION_TAG = "CloudControlApiClient";

CloudControlApiClient::CloudControlApiClient(const CloudControlApi::CloudControlApiClientConfiguration& clientConfiguration,
                                             std::shared_ptr<CloudControlApiEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudControlApiClient::CloudControlApiClient(const AWSCredentials& credentials,
                                             std::shared_ptr<CloudControlApiEndpointProviderBase> endpointProvider,
                                             const CloudControlApi::CloudControlApiClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CloudControlApiClient::CloudControlApiClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                             std::shared_ptr<CloudControlApiEndpointProviderBase> endpointProvider,
                                             const CloudControlApi::CloudControlApiClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  CloudControlApiClient::CloudControlApiClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<CloudControlApiEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudControlApiClient::CloudControlApiClient(const AWSCredentials& credentials,
                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudControlApiEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CloudControlApiClient::CloudControlApiClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CloudControlApiErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CloudControlApiEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
CloudControlApiClient::~CloudControlApiClient()
{
}

std::shared_ptr<CloudControlApiEndpointProviderBase>& CloudControlApiClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void CloudControlApiClient::init(const CloudControlApi::CloudControlApiClientConfiguration& config)
{
  AWSClient::SetServiceClientName("CloudControl");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void CloudControlApiClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CancelResourceRequestOutcome CloudControlApiClient::CancelResourceRequest(const CancelResourceRequestRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelResourceRequest, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelResourceRequest, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CancelResourceRequestOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelResourceRequestOutcomeCallable CloudControlApiClient::CancelResourceRequestCallable(const CancelResourceRequestRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::CancelResourceRequest, this, request, m_executor.get());
}

void CloudControlApiClient::CancelResourceRequestAsync(const CancelResourceRequestRequest& request, const CancelResourceRequestResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::CancelResourceRequest, this, request, handler, context, m_executor.get());
}

CreateResourceOutcome CloudControlApiClient::CreateResource(const CreateResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateResourceOutcomeCallable CloudControlApiClient::CreateResourceCallable(const CreateResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::CreateResource, this, request, m_executor.get());
}

void CloudControlApiClient::CreateResourceAsync(const CreateResourceRequest& request, const CreateResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::CreateResource, this, request, handler, context, m_executor.get());
}

DeleteResourceOutcome CloudControlApiClient::DeleteResource(const DeleteResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteResourceOutcomeCallable CloudControlApiClient::DeleteResourceCallable(const DeleteResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::DeleteResource, this, request, m_executor.get());
}

void CloudControlApiClient::DeleteResourceAsync(const DeleteResourceRequest& request, const DeleteResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::DeleteResource, this, request, handler, context, m_executor.get());
}

GetResourceOutcome CloudControlApiClient::GetResource(const GetResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetResourceOutcomeCallable CloudControlApiClient::GetResourceCallable(const GetResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::GetResource, this, request, m_executor.get());
}

void CloudControlApiClient::GetResourceAsync(const GetResourceRequest& request, const GetResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::GetResource, this, request, handler, context, m_executor.get());
}

GetResourceRequestStatusOutcome CloudControlApiClient::GetResourceRequestStatus(const GetResourceRequestStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResourceRequestStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResourceRequestStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetResourceRequestStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetResourceRequestStatusOutcomeCallable CloudControlApiClient::GetResourceRequestStatusCallable(const GetResourceRequestStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::GetResourceRequestStatus, this, request, m_executor.get());
}

void CloudControlApiClient::GetResourceRequestStatusAsync(const GetResourceRequestStatusRequest& request, const GetResourceRequestStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::GetResourceRequestStatus, this, request, handler, context, m_executor.get());
}

ListResourceRequestsOutcome CloudControlApiClient::ListResourceRequests(const ListResourceRequestsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResourceRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResourceRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListResourceRequestsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListResourceRequestsOutcomeCallable CloudControlApiClient::ListResourceRequestsCallable(const ListResourceRequestsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::ListResourceRequests, this, request, m_executor.get());
}

void CloudControlApiClient::ListResourceRequestsAsync(const ListResourceRequestsRequest& request, const ListResourceRequestsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::ListResourceRequests, this, request, handler, context, m_executor.get());
}

ListResourcesOutcome CloudControlApiClient::ListResources(const ListResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListResourcesOutcomeCallable CloudControlApiClient::ListResourcesCallable(const ListResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::ListResources, this, request, m_executor.get());
}

void CloudControlApiClient::ListResourcesAsync(const ListResourcesRequest& request, const ListResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::ListResources, this, request, handler, context, m_executor.get());
}

UpdateResourceOutcome CloudControlApiClient::UpdateResource(const UpdateResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateResourceOutcomeCallable CloudControlApiClient::UpdateResourceCallable(const UpdateResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CloudControlApiClient::UpdateResource, this, request, m_executor.get());
}

void CloudControlApiClient::UpdateResourceAsync(const UpdateResourceRequest& request, const UpdateResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CloudControlApiClient::UpdateResource, this, request, handler, context, m_executor.get());
}

