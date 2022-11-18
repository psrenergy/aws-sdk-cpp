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

#include <aws/iotsitewise/IoTSiteWiseClient.h>
#include <aws/iotsitewise/IoTSiteWiseErrorMarshaller.h>
#include <aws/iotsitewise/IoTSiteWiseEndpointProvider.h>
#include <aws/iotsitewise/model/AssociateAssetsRequest.h>
#include <aws/iotsitewise/model/AssociateTimeSeriesToAssetPropertyRequest.h>
#include <aws/iotsitewise/model/BatchAssociateProjectAssetsRequest.h>
#include <aws/iotsitewise/model/BatchDisassociateProjectAssetsRequest.h>
#include <aws/iotsitewise/model/BatchGetAssetPropertyAggregatesRequest.h>
#include <aws/iotsitewise/model/BatchGetAssetPropertyValueRequest.h>
#include <aws/iotsitewise/model/BatchGetAssetPropertyValueHistoryRequest.h>
#include <aws/iotsitewise/model/BatchPutAssetPropertyValueRequest.h>
#include <aws/iotsitewise/model/CreateAccessPolicyRequest.h>
#include <aws/iotsitewise/model/CreateAssetRequest.h>
#include <aws/iotsitewise/model/CreateAssetModelRequest.h>
#include <aws/iotsitewise/model/CreateBulkImportJobRequest.h>
#include <aws/iotsitewise/model/CreateDashboardRequest.h>
#include <aws/iotsitewise/model/CreateGatewayRequest.h>
#include <aws/iotsitewise/model/CreatePortalRequest.h>
#include <aws/iotsitewise/model/CreateProjectRequest.h>
#include <aws/iotsitewise/model/DeleteAccessPolicyRequest.h>
#include <aws/iotsitewise/model/DeleteAssetRequest.h>
#include <aws/iotsitewise/model/DeleteAssetModelRequest.h>
#include <aws/iotsitewise/model/DeleteDashboardRequest.h>
#include <aws/iotsitewise/model/DeleteGatewayRequest.h>
#include <aws/iotsitewise/model/DeletePortalRequest.h>
#include <aws/iotsitewise/model/DeleteProjectRequest.h>
#include <aws/iotsitewise/model/DeleteTimeSeriesRequest.h>
#include <aws/iotsitewise/model/DescribeAccessPolicyRequest.h>
#include <aws/iotsitewise/model/DescribeAssetRequest.h>
#include <aws/iotsitewise/model/DescribeAssetModelRequest.h>
#include <aws/iotsitewise/model/DescribeAssetPropertyRequest.h>
#include <aws/iotsitewise/model/DescribeBulkImportJobRequest.h>
#include <aws/iotsitewise/model/DescribeDashboardRequest.h>
#include <aws/iotsitewise/model/DescribeDefaultEncryptionConfigurationRequest.h>
#include <aws/iotsitewise/model/DescribeGatewayRequest.h>
#include <aws/iotsitewise/model/DescribeGatewayCapabilityConfigurationRequest.h>
#include <aws/iotsitewise/model/DescribeLoggingOptionsRequest.h>
#include <aws/iotsitewise/model/DescribePortalRequest.h>
#include <aws/iotsitewise/model/DescribeProjectRequest.h>
#include <aws/iotsitewise/model/DescribeStorageConfigurationRequest.h>
#include <aws/iotsitewise/model/DescribeTimeSeriesRequest.h>
#include <aws/iotsitewise/model/DisassociateAssetsRequest.h>
#include <aws/iotsitewise/model/DisassociateTimeSeriesFromAssetPropertyRequest.h>
#include <aws/iotsitewise/model/GetAssetPropertyAggregatesRequest.h>
#include <aws/iotsitewise/model/GetAssetPropertyValueRequest.h>
#include <aws/iotsitewise/model/GetAssetPropertyValueHistoryRequest.h>
#include <aws/iotsitewise/model/GetInterpolatedAssetPropertyValuesRequest.h>
#include <aws/iotsitewise/model/ListAccessPoliciesRequest.h>
#include <aws/iotsitewise/model/ListAssetModelPropertiesRequest.h>
#include <aws/iotsitewise/model/ListAssetModelsRequest.h>
#include <aws/iotsitewise/model/ListAssetPropertiesRequest.h>
#include <aws/iotsitewise/model/ListAssetRelationshipsRequest.h>
#include <aws/iotsitewise/model/ListAssetsRequest.h>
#include <aws/iotsitewise/model/ListAssociatedAssetsRequest.h>
#include <aws/iotsitewise/model/ListBulkImportJobsRequest.h>
#include <aws/iotsitewise/model/ListDashboardsRequest.h>
#include <aws/iotsitewise/model/ListGatewaysRequest.h>
#include <aws/iotsitewise/model/ListPortalsRequest.h>
#include <aws/iotsitewise/model/ListProjectAssetsRequest.h>
#include <aws/iotsitewise/model/ListProjectsRequest.h>
#include <aws/iotsitewise/model/ListTagsForResourceRequest.h>
#include <aws/iotsitewise/model/ListTimeSeriesRequest.h>
#include <aws/iotsitewise/model/PutDefaultEncryptionConfigurationRequest.h>
#include <aws/iotsitewise/model/PutLoggingOptionsRequest.h>
#include <aws/iotsitewise/model/PutStorageConfigurationRequest.h>
#include <aws/iotsitewise/model/TagResourceRequest.h>
#include <aws/iotsitewise/model/UntagResourceRequest.h>
#include <aws/iotsitewise/model/UpdateAccessPolicyRequest.h>
#include <aws/iotsitewise/model/UpdateAssetRequest.h>
#include <aws/iotsitewise/model/UpdateAssetModelRequest.h>
#include <aws/iotsitewise/model/UpdateAssetPropertyRequest.h>
#include <aws/iotsitewise/model/UpdateDashboardRequest.h>
#include <aws/iotsitewise/model/UpdateGatewayRequest.h>
#include <aws/iotsitewise/model/UpdateGatewayCapabilityConfigurationRequest.h>
#include <aws/iotsitewise/model/UpdatePortalRequest.h>
#include <aws/iotsitewise/model/UpdateProjectRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::IoTSiteWise;
using namespace Aws::IoTSiteWise::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* IoTSiteWiseClient::SERVICE_NAME = "iotsitewise";
const char* IoTSiteWiseClient::ALLOCATION_TAG = "IoTSiteWiseClient";

IoTSiteWiseClient::IoTSiteWiseClient(const IoTSiteWise::IoTSiteWiseClientConfiguration& clientConfiguration,
                                     std::shared_ptr<IoTSiteWiseEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IoTSiteWiseClient::IoTSiteWiseClient(const AWSCredentials& credentials,
                                     std::shared_ptr<IoTSiteWiseEndpointProviderBase> endpointProvider,
                                     const IoTSiteWise::IoTSiteWiseClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

IoTSiteWiseClient::IoTSiteWiseClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                     std::shared_ptr<IoTSiteWiseEndpointProviderBase> endpointProvider,
                                     const IoTSiteWise::IoTSiteWiseClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  IoTSiteWiseClient::IoTSiteWiseClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<IoTSiteWiseEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IoTSiteWiseClient::IoTSiteWiseClient(const AWSCredentials& credentials,
                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IoTSiteWiseEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

IoTSiteWiseClient::IoTSiteWiseClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<IoTSiteWiseErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<IoTSiteWiseEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
IoTSiteWiseClient::~IoTSiteWiseClient()
{
}

std::shared_ptr<IoTSiteWiseEndpointProviderBase>& IoTSiteWiseClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void IoTSiteWiseClient::init(const IoTSiteWise::IoTSiteWiseClientConfiguration& config)
{
  AWSClient::SetServiceClientName("IoTSiteWise");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void IoTSiteWiseClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateAssetsOutcome IoTSiteWiseClient::AssociateAssets(const AssociateAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateAssets", "Required field: AssetId, is not set");
    return AssociateAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), AssociateAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/associate");
  return AssociateAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateAssetsOutcomeCallable IoTSiteWiseClient::AssociateAssetsCallable(const AssociateAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateAssets, request, m_executor.get());
}

void IoTSiteWiseClient::AssociateAssetsAsync(const AssociateAssetsRequest& request, const AssociateAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateAssets, request, handler, context, m_executor.get());
}

AssociateTimeSeriesToAssetPropertyOutcome IoTSiteWiseClient::AssociateTimeSeriesToAssetProperty(const AssociateTimeSeriesToAssetPropertyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateTimeSeriesToAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateTimeSeriesToAssetProperty", "Required field: Alias, is not set");
    return AssociateTimeSeriesToAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Alias]", false));
  }
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateTimeSeriesToAssetProperty", "Required field: AssetId, is not set");
    return AssociateTimeSeriesToAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  if (!request.PropertyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateTimeSeriesToAssetProperty", "Required field: PropertyId, is not set");
    return AssociateTimeSeriesToAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PropertyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateTimeSeriesToAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), AssociateTimeSeriesToAssetPropertyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/timeseries/associate/");
  return AssociateTimeSeriesToAssetPropertyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateTimeSeriesToAssetPropertyOutcomeCallable IoTSiteWiseClient::AssociateTimeSeriesToAssetPropertyCallable(const AssociateTimeSeriesToAssetPropertyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateTimeSeriesToAssetProperty, request, m_executor.get());
}

void IoTSiteWiseClient::AssociateTimeSeriesToAssetPropertyAsync(const AssociateTimeSeriesToAssetPropertyRequest& request, const AssociateTimeSeriesToAssetPropertyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateTimeSeriesToAssetProperty, request, handler, context, m_executor.get());
}

BatchAssociateProjectAssetsOutcome IoTSiteWiseClient::BatchAssociateProjectAssets(const BatchAssociateProjectAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchAssociateProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchAssociateProjectAssets", "Required field: ProjectId, is not set");
    return BatchAssociateProjectAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchAssociateProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchAssociateProjectAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/associate");
  return BatchAssociateProjectAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchAssociateProjectAssetsOutcomeCallable IoTSiteWiseClient::BatchAssociateProjectAssetsCallable(const BatchAssociateProjectAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchAssociateProjectAssets, request, m_executor.get());
}

void IoTSiteWiseClient::BatchAssociateProjectAssetsAsync(const BatchAssociateProjectAssetsRequest& request, const BatchAssociateProjectAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchAssociateProjectAssets, request, handler, context, m_executor.get());
}

BatchDisassociateProjectAssetsOutcome IoTSiteWiseClient::BatchDisassociateProjectAssets(const BatchDisassociateProjectAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchDisassociateProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("BatchDisassociateProjectAssets", "Required field: ProjectId, is not set");
    return BatchDisassociateProjectAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchDisassociateProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchDisassociateProjectAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/disassociate");
  return BatchDisassociateProjectAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchDisassociateProjectAssetsOutcomeCallable IoTSiteWiseClient::BatchDisassociateProjectAssetsCallable(const BatchDisassociateProjectAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchDisassociateProjectAssets, request, m_executor.get());
}

void IoTSiteWiseClient::BatchDisassociateProjectAssetsAsync(const BatchDisassociateProjectAssetsRequest& request, const BatchDisassociateProjectAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchDisassociateProjectAssets, request, handler, context, m_executor.get());
}

BatchGetAssetPropertyAggregatesOutcome IoTSiteWiseClient::BatchGetAssetPropertyAggregates(const BatchGetAssetPropertyAggregatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetAssetPropertyAggregates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetAssetPropertyAggregates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchGetAssetPropertyAggregatesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/batch/aggregates");
  return BatchGetAssetPropertyAggregatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetAssetPropertyAggregatesOutcomeCallable IoTSiteWiseClient::BatchGetAssetPropertyAggregatesCallable(const BatchGetAssetPropertyAggregatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchGetAssetPropertyAggregates, request, m_executor.get());
}

void IoTSiteWiseClient::BatchGetAssetPropertyAggregatesAsync(const BatchGetAssetPropertyAggregatesRequest& request, const BatchGetAssetPropertyAggregatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchGetAssetPropertyAggregates, request, handler, context, m_executor.get());
}

BatchGetAssetPropertyValueOutcome IoTSiteWiseClient::BatchGetAssetPropertyValue(const BatchGetAssetPropertyValueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchGetAssetPropertyValueOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/batch/latest");
  return BatchGetAssetPropertyValueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetAssetPropertyValueOutcomeCallable IoTSiteWiseClient::BatchGetAssetPropertyValueCallable(const BatchGetAssetPropertyValueRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchGetAssetPropertyValue, request, m_executor.get());
}

void IoTSiteWiseClient::BatchGetAssetPropertyValueAsync(const BatchGetAssetPropertyValueRequest& request, const BatchGetAssetPropertyValueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchGetAssetPropertyValue, request, handler, context, m_executor.get());
}

BatchGetAssetPropertyValueHistoryOutcome IoTSiteWiseClient::BatchGetAssetPropertyValueHistory(const BatchGetAssetPropertyValueHistoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetAssetPropertyValueHistory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetAssetPropertyValueHistory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchGetAssetPropertyValueHistoryOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/batch/history");
  return BatchGetAssetPropertyValueHistoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetAssetPropertyValueHistoryOutcomeCallable IoTSiteWiseClient::BatchGetAssetPropertyValueHistoryCallable(const BatchGetAssetPropertyValueHistoryRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchGetAssetPropertyValueHistory, request, m_executor.get());
}

void IoTSiteWiseClient::BatchGetAssetPropertyValueHistoryAsync(const BatchGetAssetPropertyValueHistoryRequest& request, const BatchGetAssetPropertyValueHistoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchGetAssetPropertyValueHistory, request, handler, context, m_executor.get());
}

BatchPutAssetPropertyValueOutcome IoTSiteWiseClient::BatchPutAssetPropertyValue(const BatchPutAssetPropertyValueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchPutAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchPutAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), BatchPutAssetPropertyValueOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties");
  return BatchPutAssetPropertyValueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchPutAssetPropertyValueOutcomeCallable IoTSiteWiseClient::BatchPutAssetPropertyValueCallable(const BatchPutAssetPropertyValueRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(BatchPutAssetPropertyValue, request, m_executor.get());
}

void IoTSiteWiseClient::BatchPutAssetPropertyValueAsync(const BatchPutAssetPropertyValueRequest& request, const BatchPutAssetPropertyValueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(BatchPutAssetPropertyValue, request, handler, context, m_executor.get());
}

CreateAccessPolicyOutcome IoTSiteWiseClient::CreateAccessPolicy(const CreateAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateAccessPolicyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policies");
  return CreateAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAccessPolicyOutcomeCallable IoTSiteWiseClient::CreateAccessPolicyCallable(const CreateAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAccessPolicy, request, m_executor.get());
}

void IoTSiteWiseClient::CreateAccessPolicyAsync(const CreateAccessPolicyRequest& request, const CreateAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAccessPolicy, request, handler, context, m_executor.get());
}

CreateAssetOutcome IoTSiteWiseClient::CreateAsset(const CreateAssetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateAssetOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets");
  return CreateAssetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAssetOutcomeCallable IoTSiteWiseClient::CreateAssetCallable(const CreateAssetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAsset, request, m_executor.get());
}

void IoTSiteWiseClient::CreateAssetAsync(const CreateAssetRequest& request, const CreateAssetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAsset, request, handler, context, m_executor.get());
}

CreateAssetModelOutcome IoTSiteWiseClient::CreateAssetModel(const CreateAssetModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateAssetModelOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models");
  return CreateAssetModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAssetModelOutcomeCallable IoTSiteWiseClient::CreateAssetModelCallable(const CreateAssetModelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAssetModel, request, m_executor.get());
}

void IoTSiteWiseClient::CreateAssetModelAsync(const CreateAssetModelRequest& request, const CreateAssetModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAssetModel, request, handler, context, m_executor.get());
}

CreateBulkImportJobOutcome IoTSiteWiseClient::CreateBulkImportJob(const CreateBulkImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateBulkImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateBulkImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateBulkImportJobOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs");
  return CreateBulkImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateBulkImportJobOutcomeCallable IoTSiteWiseClient::CreateBulkImportJobCallable(const CreateBulkImportJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateBulkImportJob, request, m_executor.get());
}

void IoTSiteWiseClient::CreateBulkImportJobAsync(const CreateBulkImportJobRequest& request, const CreateBulkImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateBulkImportJob, request, handler, context, m_executor.get());
}

CreateDashboardOutcome IoTSiteWiseClient::CreateDashboard(const CreateDashboardRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateDashboardOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/dashboards");
  return CreateDashboardOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDashboardOutcomeCallable IoTSiteWiseClient::CreateDashboardCallable(const CreateDashboardRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateDashboard, request, m_executor.get());
}

void IoTSiteWiseClient::CreateDashboardAsync(const CreateDashboardRequest& request, const CreateDashboardResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateDashboard, request, handler, context, m_executor.get());
}

CreateGatewayOutcome IoTSiteWiseClient::CreateGateway(const CreateGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateGatewayOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways");
  return CreateGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGatewayOutcomeCallable IoTSiteWiseClient::CreateGatewayCallable(const CreateGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateGateway, request, m_executor.get());
}

void IoTSiteWiseClient::CreateGatewayAsync(const CreateGatewayRequest& request, const CreateGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateGateway, request, handler, context, m_executor.get());
}

CreatePortalOutcome IoTSiteWiseClient::CreatePortal(const CreatePortalRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreatePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreatePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreatePortalOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/portals");
  return CreatePortalOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreatePortalOutcomeCallable IoTSiteWiseClient::CreatePortalCallable(const CreatePortalRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreatePortal, request, m_executor.get());
}

void IoTSiteWiseClient::CreatePortalAsync(const CreatePortalRequest& request, const CreatePortalResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreatePortal, request, handler, context, m_executor.get());
}

CreateProjectOutcome IoTSiteWiseClient::CreateProject(const CreateProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), CreateProjectOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects");
  return CreateProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateProjectOutcomeCallable IoTSiteWiseClient::CreateProjectCallable(const CreateProjectRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateProject, request, m_executor.get());
}

void IoTSiteWiseClient::CreateProjectAsync(const CreateProjectRequest& request, const CreateProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateProject, request, handler, context, m_executor.get());
}

DeleteAccessPolicyOutcome IoTSiteWiseClient::DeleteAccessPolicy(const DeleteAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccessPolicyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAccessPolicy", "Required field: AccessPolicyId, is not set");
    return DeleteAccessPolicyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccessPolicyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteAccessPolicyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccessPolicyId());
  return DeleteAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAccessPolicyOutcomeCallable IoTSiteWiseClient::DeleteAccessPolicyCallable(const DeleteAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAccessPolicy, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteAccessPolicyAsync(const DeleteAccessPolicyRequest& request, const DeleteAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAccessPolicy, request, handler, context, m_executor.get());
}

DeleteAssetOutcome IoTSiteWiseClient::DeleteAsset(const DeleteAssetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAsset", "Required field: AssetId, is not set");
    return DeleteAssetOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteAssetOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  return DeleteAssetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAssetOutcomeCallable IoTSiteWiseClient::DeleteAssetCallable(const DeleteAssetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAsset, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteAssetAsync(const DeleteAssetRequest& request, const DeleteAssetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAsset, request, handler, context, m_executor.get());
}

DeleteAssetModelOutcome IoTSiteWiseClient::DeleteAssetModel(const DeleteAssetModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetModelIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAssetModel", "Required field: AssetModelId, is not set");
    return DeleteAssetModelOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetModelId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteAssetModelOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetModelId());
  return DeleteAssetModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAssetModelOutcomeCallable IoTSiteWiseClient::DeleteAssetModelCallable(const DeleteAssetModelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAssetModel, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteAssetModelAsync(const DeleteAssetModelRequest& request, const DeleteAssetModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAssetModel, request, handler, context, m_executor.get());
}

DeleteDashboardOutcome IoTSiteWiseClient::DeleteDashboard(const DeleteDashboardRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DashboardIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDashboard", "Required field: DashboardId, is not set");
    return DeleteDashboardOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DashboardId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteDashboardOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/dashboards/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDashboardId());
  return DeleteDashboardOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDashboardOutcomeCallable IoTSiteWiseClient::DeleteDashboardCallable(const DeleteDashboardRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteDashboard, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteDashboardAsync(const DeleteDashboardRequest& request, const DeleteDashboardResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteDashboard, request, handler, context, m_executor.get());
}

DeleteGatewayOutcome IoTSiteWiseClient::DeleteGateway(const DeleteGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GatewayIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGateway", "Required field: GatewayId, is not set");
    return DeleteGatewayOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GatewayId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteGatewayOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGatewayId());
  return DeleteGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteGatewayOutcomeCallable IoTSiteWiseClient::DeleteGatewayCallable(const DeleteGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteGateway, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteGatewayAsync(const DeleteGatewayRequest& request, const DeleteGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteGateway, request, handler, context, m_executor.get());
}

DeletePortalOutcome IoTSiteWiseClient::DeletePortal(const DeletePortalRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PortalIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeletePortal", "Required field: PortalId, is not set");
    return DeletePortalOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PortalId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeletePortalOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/portals/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPortalId());
  return DeletePortalOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeletePortalOutcomeCallable IoTSiteWiseClient::DeletePortalCallable(const DeletePortalRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeletePortal, request, m_executor.get());
}

void IoTSiteWiseClient::DeletePortalAsync(const DeletePortalRequest& request, const DeletePortalResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeletePortal, request, handler, context, m_executor.get());
}

DeleteProjectOutcome IoTSiteWiseClient::DeleteProject(const DeleteProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteProject", "Required field: ProjectId, is not set");
    return DeleteProjectOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteProjectOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  return DeleteProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteProjectOutcomeCallable IoTSiteWiseClient::DeleteProjectCallable(const DeleteProjectRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteProject, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteProjectAsync(const DeleteProjectRequest& request, const DeleteProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteProject, request, handler, context, m_executor.get());
}

DeleteTimeSeriesOutcome IoTSiteWiseClient::DeleteTimeSeries(const DeleteTimeSeriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DeleteTimeSeriesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/timeseries/delete/");
  return DeleteTimeSeriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteTimeSeriesOutcomeCallable IoTSiteWiseClient::DeleteTimeSeriesCallable(const DeleteTimeSeriesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteTimeSeries, request, m_executor.get());
}

void IoTSiteWiseClient::DeleteTimeSeriesAsync(const DeleteTimeSeriesRequest& request, const DeleteTimeSeriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteTimeSeries, request, handler, context, m_executor.get());
}

DescribeAccessPolicyOutcome IoTSiteWiseClient::DescribeAccessPolicy(const DescribeAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccessPolicyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAccessPolicy", "Required field: AccessPolicyId, is not set");
    return DescribeAccessPolicyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccessPolicyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeAccessPolicyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccessPolicyId());
  return DescribeAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccessPolicyOutcomeCallable IoTSiteWiseClient::DescribeAccessPolicyCallable(const DescribeAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAccessPolicy, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeAccessPolicyAsync(const DescribeAccessPolicyRequest& request, const DescribeAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAccessPolicy, request, handler, context, m_executor.get());
}

DescribeAssetOutcome IoTSiteWiseClient::DescribeAsset(const DescribeAssetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAsset", "Required field: AssetId, is not set");
    return DescribeAssetOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeAssetOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  return DescribeAssetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAssetOutcomeCallable IoTSiteWiseClient::DescribeAssetCallable(const DescribeAssetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAsset, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeAssetAsync(const DescribeAssetRequest& request, const DescribeAssetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAsset, request, handler, context, m_executor.get());
}

DescribeAssetModelOutcome IoTSiteWiseClient::DescribeAssetModel(const DescribeAssetModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetModelIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAssetModel", "Required field: AssetModelId, is not set");
    return DescribeAssetModelOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetModelId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeAssetModelOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetModelId());
  return DescribeAssetModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAssetModelOutcomeCallable IoTSiteWiseClient::DescribeAssetModelCallable(const DescribeAssetModelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAssetModel, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeAssetModelAsync(const DescribeAssetModelRequest& request, const DescribeAssetModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAssetModel, request, handler, context, m_executor.get());
}

DescribeAssetPropertyOutcome IoTSiteWiseClient::DescribeAssetProperty(const DescribeAssetPropertyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAssetProperty", "Required field: AssetId, is not set");
    return DescribeAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  if (!request.PropertyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAssetProperty", "Required field: PropertyId, is not set");
    return DescribeAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PropertyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeAssetPropertyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPropertyId());
  return DescribeAssetPropertyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAssetPropertyOutcomeCallable IoTSiteWiseClient::DescribeAssetPropertyCallable(const DescribeAssetPropertyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAssetProperty, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeAssetPropertyAsync(const DescribeAssetPropertyRequest& request, const DescribeAssetPropertyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAssetProperty, request, handler, context, m_executor.get());
}

DescribeBulkImportJobOutcome IoTSiteWiseClient::DescribeBulkImportJob(const DescribeBulkImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeBulkImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.JobIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeBulkImportJob", "Required field: JobId, is not set");
    return DescribeBulkImportJobOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [JobId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeBulkImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeBulkImportJobOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetJobId());
  return DescribeBulkImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeBulkImportJobOutcomeCallable IoTSiteWiseClient::DescribeBulkImportJobCallable(const DescribeBulkImportJobRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeBulkImportJob, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeBulkImportJobAsync(const DescribeBulkImportJobRequest& request, const DescribeBulkImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeBulkImportJob, request, handler, context, m_executor.get());
}

DescribeDashboardOutcome IoTSiteWiseClient::DescribeDashboard(const DescribeDashboardRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DashboardIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeDashboard", "Required field: DashboardId, is not set");
    return DescribeDashboardOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DashboardId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeDashboardOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/dashboards/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDashboardId());
  return DescribeDashboardOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeDashboardOutcomeCallable IoTSiteWiseClient::DescribeDashboardCallable(const DescribeDashboardRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeDashboard, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeDashboardAsync(const DescribeDashboardRequest& request, const DescribeDashboardResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeDashboard, request, handler, context, m_executor.get());
}

DescribeDefaultEncryptionConfigurationOutcome IoTSiteWiseClient::DescribeDefaultEncryptionConfiguration(const DescribeDefaultEncryptionConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDefaultEncryptionConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDefaultEncryptionConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeDefaultEncryptionConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration/account/encryption");
  return DescribeDefaultEncryptionConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeDefaultEncryptionConfigurationOutcomeCallable IoTSiteWiseClient::DescribeDefaultEncryptionConfigurationCallable(const DescribeDefaultEncryptionConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeDefaultEncryptionConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeDefaultEncryptionConfigurationAsync(const DescribeDefaultEncryptionConfigurationRequest& request, const DescribeDefaultEncryptionConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeDefaultEncryptionConfiguration, request, handler, context, m_executor.get());
}

DescribeGatewayOutcome IoTSiteWiseClient::DescribeGateway(const DescribeGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GatewayIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeGateway", "Required field: GatewayId, is not set");
    return DescribeGatewayOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GatewayId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeGatewayOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGatewayId());
  return DescribeGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeGatewayOutcomeCallable IoTSiteWiseClient::DescribeGatewayCallable(const DescribeGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeGateway, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeGatewayAsync(const DescribeGatewayRequest& request, const DescribeGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeGateway, request, handler, context, m_executor.get());
}

DescribeGatewayCapabilityConfigurationOutcome IoTSiteWiseClient::DescribeGatewayCapabilityConfiguration(const DescribeGatewayCapabilityConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeGatewayCapabilityConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GatewayIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeGatewayCapabilityConfiguration", "Required field: GatewayId, is not set");
    return DescribeGatewayCapabilityConfigurationOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GatewayId]", false));
  }
  if (!request.CapabilityNamespaceHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeGatewayCapabilityConfiguration", "Required field: CapabilityNamespace, is not set");
    return DescribeGatewayCapabilityConfigurationOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CapabilityNamespace]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeGatewayCapabilityConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeGatewayCapabilityConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGatewayId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/capability/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCapabilityNamespace());
  return DescribeGatewayCapabilityConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeGatewayCapabilityConfigurationOutcomeCallable IoTSiteWiseClient::DescribeGatewayCapabilityConfigurationCallable(const DescribeGatewayCapabilityConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeGatewayCapabilityConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeGatewayCapabilityConfigurationAsync(const DescribeGatewayCapabilityConfigurationRequest& request, const DescribeGatewayCapabilityConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeGatewayCapabilityConfiguration, request, handler, context, m_executor.get());
}

DescribeLoggingOptionsOutcome IoTSiteWiseClient::DescribeLoggingOptions(const DescribeLoggingOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeLoggingOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeLoggingOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeLoggingOptionsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/logging");
  return DescribeLoggingOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeLoggingOptionsOutcomeCallable IoTSiteWiseClient::DescribeLoggingOptionsCallable(const DescribeLoggingOptionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeLoggingOptions, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeLoggingOptionsAsync(const DescribeLoggingOptionsRequest& request, const DescribeLoggingOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeLoggingOptions, request, handler, context, m_executor.get());
}

DescribePortalOutcome IoTSiteWiseClient::DescribePortal(const DescribePortalRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PortalIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribePortal", "Required field: PortalId, is not set");
    return DescribePortalOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PortalId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribePortalOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/portals/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPortalId());
  return DescribePortalOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribePortalOutcomeCallable IoTSiteWiseClient::DescribePortalCallable(const DescribePortalRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribePortal, request, m_executor.get());
}

void IoTSiteWiseClient::DescribePortalAsync(const DescribePortalRequest& request, const DescribePortalResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribePortal, request, handler, context, m_executor.get());
}

DescribeProjectOutcome IoTSiteWiseClient::DescribeProject(const DescribeProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeProject", "Required field: ProjectId, is not set");
    return DescribeProjectOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeProjectOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  return DescribeProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeProjectOutcomeCallable IoTSiteWiseClient::DescribeProjectCallable(const DescribeProjectRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeProject, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeProjectAsync(const DescribeProjectRequest& request, const DescribeProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeProject, request, handler, context, m_executor.get());
}

DescribeStorageConfigurationOutcome IoTSiteWiseClient::DescribeStorageConfiguration(const DescribeStorageConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeStorageConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeStorageConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeStorageConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration/account/storage");
  return DescribeStorageConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeStorageConfigurationOutcomeCallable IoTSiteWiseClient::DescribeStorageConfigurationCallable(const DescribeStorageConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeStorageConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeStorageConfigurationAsync(const DescribeStorageConfigurationRequest& request, const DescribeStorageConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeStorageConfiguration, request, handler, context, m_executor.get());
}

DescribeTimeSeriesOutcome IoTSiteWiseClient::DescribeTimeSeries(const DescribeTimeSeriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DescribeTimeSeriesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/timeseries/describe/");
  return DescribeTimeSeriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeTimeSeriesOutcomeCallable IoTSiteWiseClient::DescribeTimeSeriesCallable(const DescribeTimeSeriesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeTimeSeries, request, m_executor.get());
}

void IoTSiteWiseClient::DescribeTimeSeriesAsync(const DescribeTimeSeriesRequest& request, const DescribeTimeSeriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeTimeSeries, request, handler, context, m_executor.get());
}

DisassociateAssetsOutcome IoTSiteWiseClient::DisassociateAssets(const DisassociateAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateAssets", "Required field: AssetId, is not set");
    return DisassociateAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DisassociateAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/disassociate");
  return DisassociateAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateAssetsOutcomeCallable IoTSiteWiseClient::DisassociateAssetsCallable(const DisassociateAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateAssets, request, m_executor.get());
}

void IoTSiteWiseClient::DisassociateAssetsAsync(const DisassociateAssetsRequest& request, const DisassociateAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateAssets, request, handler, context, m_executor.get());
}

DisassociateTimeSeriesFromAssetPropertyOutcome IoTSiteWiseClient::DisassociateTimeSeriesFromAssetProperty(const DisassociateTimeSeriesFromAssetPropertyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateTimeSeriesFromAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AliasHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateTimeSeriesFromAssetProperty", "Required field: Alias, is not set");
    return DisassociateTimeSeriesFromAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Alias]", false));
  }
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateTimeSeriesFromAssetProperty", "Required field: AssetId, is not set");
    return DisassociateTimeSeriesFromAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  if (!request.PropertyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateTimeSeriesFromAssetProperty", "Required field: PropertyId, is not set");
    return DisassociateTimeSeriesFromAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PropertyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateTimeSeriesFromAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), DisassociateTimeSeriesFromAssetPropertyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/timeseries/disassociate/");
  return DisassociateTimeSeriesFromAssetPropertyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateTimeSeriesFromAssetPropertyOutcomeCallable IoTSiteWiseClient::DisassociateTimeSeriesFromAssetPropertyCallable(const DisassociateTimeSeriesFromAssetPropertyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateTimeSeriesFromAssetProperty, request, m_executor.get());
}

void IoTSiteWiseClient::DisassociateTimeSeriesFromAssetPropertyAsync(const DisassociateTimeSeriesFromAssetPropertyRequest& request, const DisassociateTimeSeriesFromAssetPropertyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateTimeSeriesFromAssetProperty, request, handler, context, m_executor.get());
}

GetAssetPropertyAggregatesOutcome IoTSiteWiseClient::GetAssetPropertyAggregates(const GetAssetPropertyAggregatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAssetPropertyAggregates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AggregateTypesHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAssetPropertyAggregates", "Required field: AggregateTypes, is not set");
    return GetAssetPropertyAggregatesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AggregateTypes]", false));
  }
  if (!request.ResolutionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAssetPropertyAggregates", "Required field: Resolution, is not set");
    return GetAssetPropertyAggregatesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Resolution]", false));
  }
  if (!request.StartDateHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAssetPropertyAggregates", "Required field: StartDate, is not set");
    return GetAssetPropertyAggregatesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StartDate]", false));
  }
  if (!request.EndDateHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetAssetPropertyAggregates", "Required field: EndDate, is not set");
    return GetAssetPropertyAggregatesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EndDate]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAssetPropertyAggregates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), GetAssetPropertyAggregatesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/aggregates");
  return GetAssetPropertyAggregatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAssetPropertyAggregatesOutcomeCallable IoTSiteWiseClient::GetAssetPropertyAggregatesCallable(const GetAssetPropertyAggregatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetAssetPropertyAggregates, request, m_executor.get());
}

void IoTSiteWiseClient::GetAssetPropertyAggregatesAsync(const GetAssetPropertyAggregatesRequest& request, const GetAssetPropertyAggregatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetAssetPropertyAggregates, request, handler, context, m_executor.get());
}

GetAssetPropertyValueOutcome IoTSiteWiseClient::GetAssetPropertyValue(const GetAssetPropertyValueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAssetPropertyValue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), GetAssetPropertyValueOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/latest");
  return GetAssetPropertyValueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAssetPropertyValueOutcomeCallable IoTSiteWiseClient::GetAssetPropertyValueCallable(const GetAssetPropertyValueRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetAssetPropertyValue, request, m_executor.get());
}

void IoTSiteWiseClient::GetAssetPropertyValueAsync(const GetAssetPropertyValueRequest& request, const GetAssetPropertyValueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetAssetPropertyValue, request, handler, context, m_executor.get());
}

GetAssetPropertyValueHistoryOutcome IoTSiteWiseClient::GetAssetPropertyValueHistory(const GetAssetPropertyValueHistoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetAssetPropertyValueHistory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetAssetPropertyValueHistory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), GetAssetPropertyValueHistoryOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/history");
  return GetAssetPropertyValueHistoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetAssetPropertyValueHistoryOutcomeCallable IoTSiteWiseClient::GetAssetPropertyValueHistoryCallable(const GetAssetPropertyValueHistoryRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetAssetPropertyValueHistory, request, m_executor.get());
}

void IoTSiteWiseClient::GetAssetPropertyValueHistoryAsync(const GetAssetPropertyValueHistoryRequest& request, const GetAssetPropertyValueHistoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetAssetPropertyValueHistory, request, handler, context, m_executor.get());
}

GetInterpolatedAssetPropertyValuesOutcome IoTSiteWiseClient::GetInterpolatedAssetPropertyValues(const GetInterpolatedAssetPropertyValuesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetInterpolatedAssetPropertyValues, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.StartTimeInSecondsHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInterpolatedAssetPropertyValues", "Required field: StartTimeInSeconds, is not set");
    return GetInterpolatedAssetPropertyValuesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [StartTimeInSeconds]", false));
  }
  if (!request.EndTimeInSecondsHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInterpolatedAssetPropertyValues", "Required field: EndTimeInSeconds, is not set");
    return GetInterpolatedAssetPropertyValuesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EndTimeInSeconds]", false));
  }
  if (!request.QualityHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInterpolatedAssetPropertyValues", "Required field: Quality, is not set");
    return GetInterpolatedAssetPropertyValuesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Quality]", false));
  }
  if (!request.IntervalInSecondsHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInterpolatedAssetPropertyValues", "Required field: IntervalInSeconds, is not set");
    return GetInterpolatedAssetPropertyValuesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [IntervalInSeconds]", false));
  }
  if (!request.TypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetInterpolatedAssetPropertyValues", "Required field: Type, is not set");
    return GetInterpolatedAssetPropertyValuesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Type]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetInterpolatedAssetPropertyValues, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), GetInterpolatedAssetPropertyValuesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/interpolated");
  return GetInterpolatedAssetPropertyValuesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetInterpolatedAssetPropertyValuesOutcomeCallable IoTSiteWiseClient::GetInterpolatedAssetPropertyValuesCallable(const GetInterpolatedAssetPropertyValuesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetInterpolatedAssetPropertyValues, request, m_executor.get());
}

void IoTSiteWiseClient::GetInterpolatedAssetPropertyValuesAsync(const GetInterpolatedAssetPropertyValuesRequest& request, const GetInterpolatedAssetPropertyValuesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetInterpolatedAssetPropertyValues, request, handler, context, m_executor.get());
}

ListAccessPoliciesOutcome IoTSiteWiseClient::ListAccessPolicies(const ListAccessPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAccessPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAccessPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAccessPoliciesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policies");
  return ListAccessPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAccessPoliciesOutcomeCallable IoTSiteWiseClient::ListAccessPoliciesCallable(const ListAccessPoliciesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAccessPolicies, request, m_executor.get());
}

void IoTSiteWiseClient::ListAccessPoliciesAsync(const ListAccessPoliciesRequest& request, const ListAccessPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAccessPolicies, request, handler, context, m_executor.get());
}

ListAssetModelPropertiesOutcome IoTSiteWiseClient::ListAssetModelProperties(const ListAssetModelPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssetModelProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetModelIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssetModelProperties", "Required field: AssetModelId, is not set");
    return ListAssetModelPropertiesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetModelId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssetModelProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssetModelPropertiesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetModelId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties");
  return ListAssetModelPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssetModelPropertiesOutcomeCallable IoTSiteWiseClient::ListAssetModelPropertiesCallable(const ListAssetModelPropertiesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssetModelProperties, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssetModelPropertiesAsync(const ListAssetModelPropertiesRequest& request, const ListAssetModelPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssetModelProperties, request, handler, context, m_executor.get());
}

ListAssetModelsOutcome IoTSiteWiseClient::ListAssetModels(const ListAssetModelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssetModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssetModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssetModelsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models");
  return ListAssetModelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssetModelsOutcomeCallable IoTSiteWiseClient::ListAssetModelsCallable(const ListAssetModelsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssetModels, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssetModelsAsync(const ListAssetModelsRequest& request, const ListAssetModelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssetModels, request, handler, context, m_executor.get());
}

ListAssetPropertiesOutcome IoTSiteWiseClient::ListAssetProperties(const ListAssetPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssetProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssetProperties", "Required field: AssetId, is not set");
    return ListAssetPropertiesOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssetProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssetPropertiesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties");
  return ListAssetPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssetPropertiesOutcomeCallable IoTSiteWiseClient::ListAssetPropertiesCallable(const ListAssetPropertiesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssetProperties, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssetPropertiesAsync(const ListAssetPropertiesRequest& request, const ListAssetPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssetProperties, request, handler, context, m_executor.get());
}

ListAssetRelationshipsOutcome IoTSiteWiseClient::ListAssetRelationships(const ListAssetRelationshipsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssetRelationships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssetRelationships", "Required field: AssetId, is not set");
    return ListAssetRelationshipsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  if (!request.TraversalTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssetRelationships", "Required field: TraversalType, is not set");
    return ListAssetRelationshipsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TraversalType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssetRelationships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssetRelationshipsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/assetRelationships");
  return ListAssetRelationshipsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssetRelationshipsOutcomeCallable IoTSiteWiseClient::ListAssetRelationshipsCallable(const ListAssetRelationshipsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssetRelationships, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssetRelationshipsAsync(const ListAssetRelationshipsRequest& request, const ListAssetRelationshipsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssetRelationships, request, handler, context, m_executor.get());
}

ListAssetsOutcome IoTSiteWiseClient::ListAssets(const ListAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets");
  return ListAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssetsOutcomeCallable IoTSiteWiseClient::ListAssetsCallable(const ListAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssets, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssetsAsync(const ListAssetsRequest& request, const ListAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssets, request, handler, context, m_executor.get());
}

ListAssociatedAssetsOutcome IoTSiteWiseClient::ListAssociatedAssets(const ListAssociatedAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssociatedAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssociatedAssets", "Required field: AssetId, is not set");
    return ListAssociatedAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssociatedAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListAssociatedAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/hierarchies");
  return ListAssociatedAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssociatedAssetsOutcomeCallable IoTSiteWiseClient::ListAssociatedAssetsCallable(const ListAssociatedAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAssociatedAssets, request, m_executor.get());
}

void IoTSiteWiseClient::ListAssociatedAssetsAsync(const ListAssociatedAssetsRequest& request, const ListAssociatedAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAssociatedAssets, request, handler, context, m_executor.get());
}

ListBulkImportJobsOutcome IoTSiteWiseClient::ListBulkImportJobs(const ListBulkImportJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListBulkImportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListBulkImportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("data.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListBulkImportJobsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/jobs");
  return ListBulkImportJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListBulkImportJobsOutcomeCallable IoTSiteWiseClient::ListBulkImportJobsCallable(const ListBulkImportJobsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListBulkImportJobs, request, m_executor.get());
}

void IoTSiteWiseClient::ListBulkImportJobsAsync(const ListBulkImportJobsRequest& request, const ListBulkImportJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListBulkImportJobs, request, handler, context, m_executor.get());
}

ListDashboardsOutcome IoTSiteWiseClient::ListDashboards(const ListDashboardsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDashboards, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDashboards", "Required field: ProjectId, is not set");
    return ListDashboardsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDashboards, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListDashboardsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/dashboards");
  return ListDashboardsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListDashboardsOutcomeCallable IoTSiteWiseClient::ListDashboardsCallable(const ListDashboardsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListDashboards, request, m_executor.get());
}

void IoTSiteWiseClient::ListDashboardsAsync(const ListDashboardsRequest& request, const ListDashboardsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListDashboards, request, handler, context, m_executor.get());
}

ListGatewaysOutcome IoTSiteWiseClient::ListGateways(const ListGatewaysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGateways, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGateways, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListGatewaysOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways");
  return ListGatewaysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListGatewaysOutcomeCallable IoTSiteWiseClient::ListGatewaysCallable(const ListGatewaysRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListGateways, request, m_executor.get());
}

void IoTSiteWiseClient::ListGatewaysAsync(const ListGatewaysRequest& request, const ListGatewaysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListGateways, request, handler, context, m_executor.get());
}

ListPortalsOutcome IoTSiteWiseClient::ListPortals(const ListPortalsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPortals, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPortals, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListPortalsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/portals");
  return ListPortalsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListPortalsOutcomeCallable IoTSiteWiseClient::ListPortalsCallable(const ListPortalsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListPortals, request, m_executor.get());
}

void IoTSiteWiseClient::ListPortalsAsync(const ListPortalsRequest& request, const ListPortalsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListPortals, request, handler, context, m_executor.get());
}

ListProjectAssetsOutcome IoTSiteWiseClient::ListProjectAssets(const ListProjectAssetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListProjectAssets", "Required field: ProjectId, is not set");
    return ListProjectAssetsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListProjectAssets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListProjectAssetsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets");
  return ListProjectAssetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListProjectAssetsOutcomeCallable IoTSiteWiseClient::ListProjectAssetsCallable(const ListProjectAssetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListProjectAssets, request, m_executor.get());
}

void IoTSiteWiseClient::ListProjectAssetsAsync(const ListProjectAssetsRequest& request, const ListProjectAssetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListProjectAssets, request, handler, context, m_executor.get());
}

ListProjectsOutcome IoTSiteWiseClient::ListProjects(const ListProjectsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PortalIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListProjects", "Required field: PortalId, is not set");
    return ListProjectsOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PortalId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListProjectsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects");
  return ListProjectsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListProjectsOutcomeCallable IoTSiteWiseClient::ListProjectsCallable(const ListProjectsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListProjects, request, m_executor.get());
}

void IoTSiteWiseClient::ListProjectsAsync(const ListProjectsRequest& request, const ListProjectsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListProjects, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome IoTSiteWiseClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListTagsForResourceOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable IoTSiteWiseClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void IoTSiteWiseClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

ListTimeSeriesOutcome IoTSiteWiseClient::ListTimeSeries(const ListTimeSeriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTimeSeries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), ListTimeSeriesOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/timeseries/");
  return ListTimeSeriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTimeSeriesOutcomeCallable IoTSiteWiseClient::ListTimeSeriesCallable(const ListTimeSeriesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTimeSeries, request, m_executor.get());
}

void IoTSiteWiseClient::ListTimeSeriesAsync(const ListTimeSeriesRequest& request, const ListTimeSeriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTimeSeries, request, handler, context, m_executor.get());
}

PutDefaultEncryptionConfigurationOutcome IoTSiteWiseClient::PutDefaultEncryptionConfiguration(const PutDefaultEncryptionConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutDefaultEncryptionConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutDefaultEncryptionConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), PutDefaultEncryptionConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration/account/encryption");
  return PutDefaultEncryptionConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutDefaultEncryptionConfigurationOutcomeCallable IoTSiteWiseClient::PutDefaultEncryptionConfigurationCallable(const PutDefaultEncryptionConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutDefaultEncryptionConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::PutDefaultEncryptionConfigurationAsync(const PutDefaultEncryptionConfigurationRequest& request, const PutDefaultEncryptionConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutDefaultEncryptionConfiguration, request, handler, context, m_executor.get());
}

PutLoggingOptionsOutcome IoTSiteWiseClient::PutLoggingOptions(const PutLoggingOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutLoggingOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutLoggingOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), PutLoggingOptionsOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/logging");
  return PutLoggingOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutLoggingOptionsOutcomeCallable IoTSiteWiseClient::PutLoggingOptionsCallable(const PutLoggingOptionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutLoggingOptions, request, m_executor.get());
}

void IoTSiteWiseClient::PutLoggingOptionsAsync(const PutLoggingOptionsRequest& request, const PutLoggingOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutLoggingOptions, request, handler, context, m_executor.get());
}

PutStorageConfigurationOutcome IoTSiteWiseClient::PutStorageConfiguration(const PutStorageConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutStorageConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutStorageConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), PutStorageConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/configuration/account/storage");
  return PutStorageConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutStorageConfigurationOutcomeCallable IoTSiteWiseClient::PutStorageConfigurationCallable(const PutStorageConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutStorageConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::PutStorageConfigurationAsync(const PutStorageConfigurationRequest& request, const PutStorageConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutStorageConfiguration, request, handler, context, m_executor.get());
}

TagResourceOutcome IoTSiteWiseClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), TagResourceOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable IoTSiteWiseClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void IoTSiteWiseClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome IoTSiteWiseClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UntagResourceOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags");
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable IoTSiteWiseClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void IoTSiteWiseClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateAccessPolicyOutcome IoTSiteWiseClient::UpdateAccessPolicy(const UpdateAccessPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AccessPolicyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAccessPolicy", "Required field: AccessPolicyId, is not set");
    return UpdateAccessPolicyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AccessPolicyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAccessPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateAccessPolicyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/access-policies/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAccessPolicyId());
  return UpdateAccessPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateAccessPolicyOutcomeCallable IoTSiteWiseClient::UpdateAccessPolicyCallable(const UpdateAccessPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAccessPolicy, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateAccessPolicyAsync(const UpdateAccessPolicyRequest& request, const UpdateAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAccessPolicy, request, handler, context, m_executor.get());
}

UpdateAssetOutcome IoTSiteWiseClient::UpdateAsset(const UpdateAssetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAsset", "Required field: AssetId, is not set");
    return UpdateAssetOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAsset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateAssetOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  return UpdateAssetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateAssetOutcomeCallable IoTSiteWiseClient::UpdateAssetCallable(const UpdateAssetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAsset, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateAssetAsync(const UpdateAssetRequest& request, const UpdateAssetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAsset, request, handler, context, m_executor.get());
}

UpdateAssetModelOutcome IoTSiteWiseClient::UpdateAssetModel(const UpdateAssetModelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetModelIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAssetModel", "Required field: AssetModelId, is not set");
    return UpdateAssetModelOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetModelId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAssetModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateAssetModelOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/asset-models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetModelId());
  return UpdateAssetModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateAssetModelOutcomeCallable IoTSiteWiseClient::UpdateAssetModelCallable(const UpdateAssetModelRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAssetModel, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateAssetModelAsync(const UpdateAssetModelRequest& request, const UpdateAssetModelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAssetModel, request, handler, context, m_executor.get());
}

UpdateAssetPropertyOutcome IoTSiteWiseClient::UpdateAssetProperty(const UpdateAssetPropertyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AssetIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAssetProperty", "Required field: AssetId, is not set");
    return UpdateAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AssetId]", false));
  }
  if (!request.PropertyIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAssetProperty", "Required field: PropertyId, is not set");
    return UpdateAssetPropertyOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PropertyId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAssetProperty, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateAssetPropertyOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/assets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAssetId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/properties/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPropertyId());
  return UpdateAssetPropertyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateAssetPropertyOutcomeCallable IoTSiteWiseClient::UpdateAssetPropertyCallable(const UpdateAssetPropertyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAssetProperty, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateAssetPropertyAsync(const UpdateAssetPropertyRequest& request, const UpdateAssetPropertyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAssetProperty, request, handler, context, m_executor.get());
}

UpdateDashboardOutcome IoTSiteWiseClient::UpdateDashboard(const UpdateDashboardRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DashboardIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDashboard", "Required field: DashboardId, is not set");
    return UpdateDashboardOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DashboardId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDashboard, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateDashboardOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/dashboards/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDashboardId());
  return UpdateDashboardOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateDashboardOutcomeCallable IoTSiteWiseClient::UpdateDashboardCallable(const UpdateDashboardRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateDashboard, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateDashboardAsync(const UpdateDashboardRequest& request, const UpdateDashboardResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateDashboard, request, handler, context, m_executor.get());
}

UpdateGatewayOutcome IoTSiteWiseClient::UpdateGateway(const UpdateGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GatewayIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGateway", "Required field: GatewayId, is not set");
    return UpdateGatewayOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GatewayId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateGatewayOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGatewayId());
  return UpdateGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateGatewayOutcomeCallable IoTSiteWiseClient::UpdateGatewayCallable(const UpdateGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGateway, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateGatewayAsync(const UpdateGatewayRequest& request, const UpdateGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGateway, request, handler, context, m_executor.get());
}

UpdateGatewayCapabilityConfigurationOutcome IoTSiteWiseClient::UpdateGatewayCapabilityConfiguration(const UpdateGatewayCapabilityConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGatewayCapabilityConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GatewayIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGatewayCapabilityConfiguration", "Required field: GatewayId, is not set");
    return UpdateGatewayCapabilityConfigurationOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GatewayId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGatewayCapabilityConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("api.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateGatewayCapabilityConfigurationOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/20200301/gateways/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGatewayId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/capability");
  return UpdateGatewayCapabilityConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGatewayCapabilityConfigurationOutcomeCallable IoTSiteWiseClient::UpdateGatewayCapabilityConfigurationCallable(const UpdateGatewayCapabilityConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGatewayCapabilityConfiguration, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateGatewayCapabilityConfigurationAsync(const UpdateGatewayCapabilityConfigurationRequest& request, const UpdateGatewayCapabilityConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGatewayCapabilityConfiguration, request, handler, context, m_executor.get());
}

UpdatePortalOutcome IoTSiteWiseClient::UpdatePortal(const UpdatePortalRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdatePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PortalIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdatePortal", "Required field: PortalId, is not set");
    return UpdatePortalOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PortalId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdatePortal, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdatePortalOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/portals/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPortalId());
  return UpdatePortalOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdatePortalOutcomeCallable IoTSiteWiseClient::UpdatePortalCallable(const UpdatePortalRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdatePortal, request, m_executor.get());
}

void IoTSiteWiseClient::UpdatePortalAsync(const UpdatePortalRequest& request, const UpdatePortalResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdatePortal, request, handler, context, m_executor.get());
}

UpdateProjectOutcome IoTSiteWiseClient::UpdateProject(const UpdateProjectRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateProject", "Required field: ProjectId, is not set");
    return UpdateProjectOutcome(Aws::Client::AWSError<IoTSiteWiseErrors>(IoTSiteWiseErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  auto addPrefixErr = endpointResolutionOutcome.GetResult().AddPrefixIfMissing("monitor.");
  AWS_CHECK(SERVICE_NAME, !addPrefixErr, addPrefixErr->GetMessage(), UpdateProjectOutcome(addPrefixErr.value()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectId());
  return UpdateProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateProjectOutcomeCallable IoTSiteWiseClient::UpdateProjectCallable(const UpdateProjectRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateProject, request, m_executor.get());
}

void IoTSiteWiseClient::UpdateProjectAsync(const UpdateProjectRequest& request, const UpdateProjectResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateProject, request, handler, context, m_executor.get());
}

