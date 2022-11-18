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

#include <aws/snowball/SnowballClient.h>
#include <aws/snowball/SnowballErrorMarshaller.h>
#include <aws/snowball/SnowballEndpointProvider.h>
#include <aws/snowball/model/CancelClusterRequest.h>
#include <aws/snowball/model/CancelJobRequest.h>
#include <aws/snowball/model/CreateAddressRequest.h>
#include <aws/snowball/model/CreateClusterRequest.h>
#include <aws/snowball/model/CreateJobRequest.h>
#include <aws/snowball/model/CreateLongTermPricingRequest.h>
#include <aws/snowball/model/CreateReturnShippingLabelRequest.h>
#include <aws/snowball/model/DescribeAddressRequest.h>
#include <aws/snowball/model/DescribeAddressesRequest.h>
#include <aws/snowball/model/DescribeClusterRequest.h>
#include <aws/snowball/model/DescribeJobRequest.h>
#include <aws/snowball/model/DescribeReturnShippingLabelRequest.h>
#include <aws/snowball/model/GetJobManifestRequest.h>
#include <aws/snowball/model/GetJobUnlockCodeRequest.h>
#include <aws/snowball/model/GetSnowballUsageRequest.h>
#include <aws/snowball/model/GetSoftwareUpdatesRequest.h>
#include <aws/snowball/model/ListClusterJobsRequest.h>
#include <aws/snowball/model/ListClustersRequest.h>
#include <aws/snowball/model/ListCompatibleImagesRequest.h>
#include <aws/snowball/model/ListJobsRequest.h>
#include <aws/snowball/model/ListLongTermPricingRequest.h>
#include <aws/snowball/model/UpdateClusterRequest.h>
#include <aws/snowball/model/UpdateJobRequest.h>
#include <aws/snowball/model/UpdateJobShipmentStateRequest.h>
#include <aws/snowball/model/UpdateLongTermPricingRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Snowball;
using namespace Aws::Snowball::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* SnowballClient::SERVICE_NAME = "snowball";
const char* SnowballClient::ALLOCATION_TAG = "SnowballClient";

SnowballClient::SnowballClient(const Snowball::SnowballClientConfiguration& clientConfiguration,
                               std::shared_ptr<SnowballEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

SnowballClient::SnowballClient(const AWSCredentials& credentials,
                               std::shared_ptr<SnowballEndpointProviderBase> endpointProvider,
                               const Snowball::SnowballClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

SnowballClient::SnowballClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<SnowballEndpointProviderBase> endpointProvider,
                               const Snowball::SnowballClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  SnowballClient::SnowballClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<SnowballEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

SnowballClient::SnowballClient(const AWSCredentials& credentials,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<SnowballEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

SnowballClient::SnowballClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SnowballErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<SnowballEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
SnowballClient::~SnowballClient()
{
}

std::shared_ptr<SnowballEndpointProviderBase>& SnowballClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void SnowballClient::init(const Snowball::SnowballClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Snowball");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void SnowballClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CancelClusterOutcome SnowballClient::CancelCluster(const CancelClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CancelClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelClusterOutcomeCallable SnowballClient::CancelClusterCallable(const CancelClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CancelCluster, request, m_executor.get());
}

void SnowballClient::CancelClusterAsync(const CancelClusterRequest& request, const CancelClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CancelCluster, request, handler, context, m_executor.get());
}

CancelJobOutcome SnowballClient::CancelJob(const CancelJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CancelJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelJobOutcomeCallable SnowballClient::CancelJobCallable(const CancelJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CancelJob, request, m_executor.get());
}

void SnowballClient::CancelJobAsync(const CancelJobRequest& request, const CancelJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CancelJob, request, handler, context, m_executor.get());
}

CreateAddressOutcome SnowballClient::CreateAddress(const CreateAddressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAddress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAddress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateAddressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAddressOutcomeCallable SnowballClient::CreateAddressCallable(const CreateAddressRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAddress, request, m_executor.get());
}

void SnowballClient::CreateAddressAsync(const CreateAddressRequest& request, const CreateAddressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAddress, request, handler, context, m_executor.get());
}

CreateClusterOutcome SnowballClient::CreateCluster(const CreateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateClusterOutcomeCallable SnowballClient::CreateClusterCallable(const CreateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateCluster, request, m_executor.get());
}

void SnowballClient::CreateClusterAsync(const CreateClusterRequest& request, const CreateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateCluster, request, handler, context, m_executor.get());
}

CreateJobOutcome SnowballClient::CreateJob(const CreateJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateJobOutcomeCallable SnowballClient::CreateJobCallable(const CreateJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateJob, request, m_executor.get());
}

void SnowballClient::CreateJobAsync(const CreateJobRequest& request, const CreateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateJob, request, handler, context, m_executor.get());
}

CreateLongTermPricingOutcome SnowballClient::CreateLongTermPricing(const CreateLongTermPricingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateLongTermPricingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateLongTermPricingOutcomeCallable SnowballClient::CreateLongTermPricingCallable(const CreateLongTermPricingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateLongTermPricing, request, m_executor.get());
}

void SnowballClient::CreateLongTermPricingAsync(const CreateLongTermPricingRequest& request, const CreateLongTermPricingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateLongTermPricing, request, handler, context, m_executor.get());
}

CreateReturnShippingLabelOutcome SnowballClient::CreateReturnShippingLabel(const CreateReturnShippingLabelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateReturnShippingLabel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateReturnShippingLabel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateReturnShippingLabelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateReturnShippingLabelOutcomeCallable SnowballClient::CreateReturnShippingLabelCallable(const CreateReturnShippingLabelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateReturnShippingLabel, request, m_executor.get());
}

void SnowballClient::CreateReturnShippingLabelAsync(const CreateReturnShippingLabelRequest& request, const CreateReturnShippingLabelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateReturnShippingLabel, request, handler, context, m_executor.get());
}

DescribeAddressOutcome SnowballClient::DescribeAddress(const DescribeAddressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAddress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAddress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAddressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAddressOutcomeCallable SnowballClient::DescribeAddressCallable(const DescribeAddressRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAddress, request, m_executor.get());
}

void SnowballClient::DescribeAddressAsync(const DescribeAddressRequest& request, const DescribeAddressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAddress, request, handler, context, m_executor.get());
}

DescribeAddressesOutcome SnowballClient::DescribeAddresses(const DescribeAddressesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAddresses, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAddresses, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAddressesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAddressesOutcomeCallable SnowballClient::DescribeAddressesCallable(const DescribeAddressesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAddresses, request, m_executor.get());
}

void SnowballClient::DescribeAddressesAsync(const DescribeAddressesRequest& request, const DescribeAddressesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAddresses, request, handler, context, m_executor.get());
}

DescribeClusterOutcome SnowballClient::DescribeCluster(const DescribeClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeClusterOutcomeCallable SnowballClient::DescribeClusterCallable(const DescribeClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeCluster, request, m_executor.get());
}

void SnowballClient::DescribeClusterAsync(const DescribeClusterRequest& request, const DescribeClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeCluster, request, handler, context, m_executor.get());
}

DescribeJobOutcome SnowballClient::DescribeJob(const DescribeJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeJobOutcomeCallable SnowballClient::DescribeJobCallable(const DescribeJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeJob, request, m_executor.get());
}

void SnowballClient::DescribeJobAsync(const DescribeJobRequest& request, const DescribeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeJob, request, handler, context, m_executor.get());
}

DescribeReturnShippingLabelOutcome SnowballClient::DescribeReturnShippingLabel(const DescribeReturnShippingLabelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReturnShippingLabel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReturnShippingLabel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReturnShippingLabelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeReturnShippingLabelOutcomeCallable SnowballClient::DescribeReturnShippingLabelCallable(const DescribeReturnShippingLabelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeReturnShippingLabel, request, m_executor.get());
}

void SnowballClient::DescribeReturnShippingLabelAsync(const DescribeReturnShippingLabelRequest& request, const DescribeReturnShippingLabelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeReturnShippingLabel, request, handler, context, m_executor.get());
}

GetJobManifestOutcome SnowballClient::GetJobManifest(const GetJobManifestRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetJobManifest, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetJobManifest, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetJobManifestOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetJobManifestOutcomeCallable SnowballClient::GetJobManifestCallable(const GetJobManifestRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetJobManifest, request, m_executor.get());
}

void SnowballClient::GetJobManifestAsync(const GetJobManifestRequest& request, const GetJobManifestResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetJobManifest, request, handler, context, m_executor.get());
}

GetJobUnlockCodeOutcome SnowballClient::GetJobUnlockCode(const GetJobUnlockCodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetJobUnlockCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetJobUnlockCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetJobUnlockCodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetJobUnlockCodeOutcomeCallable SnowballClient::GetJobUnlockCodeCallable(const GetJobUnlockCodeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetJobUnlockCode, request, m_executor.get());
}

void SnowballClient::GetJobUnlockCodeAsync(const GetJobUnlockCodeRequest& request, const GetJobUnlockCodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetJobUnlockCode, request, handler, context, m_executor.get());
}

GetSnowballUsageOutcome SnowballClient::GetSnowballUsage(const GetSnowballUsageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSnowballUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSnowballUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSnowballUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSnowballUsageOutcomeCallable SnowballClient::GetSnowballUsageCallable(const GetSnowballUsageRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSnowballUsage, request, m_executor.get());
}

void SnowballClient::GetSnowballUsageAsync(const GetSnowballUsageRequest& request, const GetSnowballUsageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSnowballUsage, request, handler, context, m_executor.get());
}

GetSoftwareUpdatesOutcome SnowballClient::GetSoftwareUpdates(const GetSoftwareUpdatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSoftwareUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSoftwareUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSoftwareUpdatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSoftwareUpdatesOutcomeCallable SnowballClient::GetSoftwareUpdatesCallable(const GetSoftwareUpdatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSoftwareUpdates, request, m_executor.get());
}

void SnowballClient::GetSoftwareUpdatesAsync(const GetSoftwareUpdatesRequest& request, const GetSoftwareUpdatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSoftwareUpdates, request, handler, context, m_executor.get());
}

ListClusterJobsOutcome SnowballClient::ListClusterJobs(const ListClusterJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListClusterJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListClusterJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListClusterJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListClusterJobsOutcomeCallable SnowballClient::ListClusterJobsCallable(const ListClusterJobsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListClusterJobs, request, m_executor.get());
}

void SnowballClient::ListClusterJobsAsync(const ListClusterJobsRequest& request, const ListClusterJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListClusterJobs, request, handler, context, m_executor.get());
}

ListClustersOutcome SnowballClient::ListClusters(const ListClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListClustersOutcomeCallable SnowballClient::ListClustersCallable(const ListClustersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListClusters, request, m_executor.get());
}

void SnowballClient::ListClustersAsync(const ListClustersRequest& request, const ListClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListClusters, request, handler, context, m_executor.get());
}

ListCompatibleImagesOutcome SnowballClient::ListCompatibleImages(const ListCompatibleImagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCompatibleImages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCompatibleImages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListCompatibleImagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListCompatibleImagesOutcomeCallable SnowballClient::ListCompatibleImagesCallable(const ListCompatibleImagesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListCompatibleImages, request, m_executor.get());
}

void SnowballClient::ListCompatibleImagesAsync(const ListCompatibleImagesRequest& request, const ListCompatibleImagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListCompatibleImages, request, handler, context, m_executor.get());
}

ListJobsOutcome SnowballClient::ListJobs(const ListJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListJobsOutcomeCallable SnowballClient::ListJobsCallable(const ListJobsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListJobs, request, m_executor.get());
}

void SnowballClient::ListJobsAsync(const ListJobsRequest& request, const ListJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListJobs, request, handler, context, m_executor.get());
}

ListLongTermPricingOutcome SnowballClient::ListLongTermPricing(const ListLongTermPricingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListLongTermPricingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListLongTermPricingOutcomeCallable SnowballClient::ListLongTermPricingCallable(const ListLongTermPricingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListLongTermPricing, request, m_executor.get());
}

void SnowballClient::ListLongTermPricingAsync(const ListLongTermPricingRequest& request, const ListLongTermPricingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListLongTermPricing, request, handler, context, m_executor.get());
}

UpdateClusterOutcome SnowballClient::UpdateCluster(const UpdateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateClusterOutcomeCallable SnowballClient::UpdateClusterCallable(const UpdateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCluster, request, m_executor.get());
}

void SnowballClient::UpdateClusterAsync(const UpdateClusterRequest& request, const UpdateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCluster, request, handler, context, m_executor.get());
}

UpdateJobOutcome SnowballClient::UpdateJob(const UpdateJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateJobOutcomeCallable SnowballClient::UpdateJobCallable(const UpdateJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateJob, request, m_executor.get());
}

void SnowballClient::UpdateJobAsync(const UpdateJobRequest& request, const UpdateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateJob, request, handler, context, m_executor.get());
}

UpdateJobShipmentStateOutcome SnowballClient::UpdateJobShipmentState(const UpdateJobShipmentStateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateJobShipmentState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateJobShipmentState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateJobShipmentStateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateJobShipmentStateOutcomeCallable SnowballClient::UpdateJobShipmentStateCallable(const UpdateJobShipmentStateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateJobShipmentState, request, m_executor.get());
}

void SnowballClient::UpdateJobShipmentStateAsync(const UpdateJobShipmentStateRequest& request, const UpdateJobShipmentStateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateJobShipmentState, request, handler, context, m_executor.get());
}

UpdateLongTermPricingOutcome SnowballClient::UpdateLongTermPricing(const UpdateLongTermPricingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateLongTermPricing, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateLongTermPricingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateLongTermPricingOutcomeCallable SnowballClient::UpdateLongTermPricingCallable(const UpdateLongTermPricingRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateLongTermPricing, request, m_executor.get());
}

void SnowballClient::UpdateLongTermPricingAsync(const UpdateLongTermPricingRequest& request, const UpdateLongTermPricingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateLongTermPricing, request, handler, context, m_executor.get());
}

