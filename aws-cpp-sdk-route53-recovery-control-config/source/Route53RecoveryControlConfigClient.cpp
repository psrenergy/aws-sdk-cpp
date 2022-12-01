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

#include <aws/route53-recovery-control-config/Route53RecoveryControlConfigClient.h>
#include <aws/route53-recovery-control-config/Route53RecoveryControlConfigErrorMarshaller.h>
#include <aws/route53-recovery-control-config/Route53RecoveryControlConfigEndpointProvider.h>
#include <aws/route53-recovery-control-config/model/CreateClusterRequest.h>
#include <aws/route53-recovery-control-config/model/CreateControlPanelRequest.h>
#include <aws/route53-recovery-control-config/model/CreateRoutingControlRequest.h>
#include <aws/route53-recovery-control-config/model/CreateSafetyRuleRequest.h>
#include <aws/route53-recovery-control-config/model/DeleteClusterRequest.h>
#include <aws/route53-recovery-control-config/model/DeleteControlPanelRequest.h>
#include <aws/route53-recovery-control-config/model/DeleteRoutingControlRequest.h>
#include <aws/route53-recovery-control-config/model/DeleteSafetyRuleRequest.h>
#include <aws/route53-recovery-control-config/model/DescribeClusterRequest.h>
#include <aws/route53-recovery-control-config/model/DescribeControlPanelRequest.h>
#include <aws/route53-recovery-control-config/model/DescribeRoutingControlRequest.h>
#include <aws/route53-recovery-control-config/model/DescribeSafetyRuleRequest.h>
#include <aws/route53-recovery-control-config/model/ListAssociatedRoute53HealthChecksRequest.h>
#include <aws/route53-recovery-control-config/model/ListClustersRequest.h>
#include <aws/route53-recovery-control-config/model/ListControlPanelsRequest.h>
#include <aws/route53-recovery-control-config/model/ListRoutingControlsRequest.h>
#include <aws/route53-recovery-control-config/model/ListSafetyRulesRequest.h>
#include <aws/route53-recovery-control-config/model/ListTagsForResourceRequest.h>
#include <aws/route53-recovery-control-config/model/TagResourceRequest.h>
#include <aws/route53-recovery-control-config/model/UntagResourceRequest.h>
#include <aws/route53-recovery-control-config/model/UpdateControlPanelRequest.h>
#include <aws/route53-recovery-control-config/model/UpdateRoutingControlRequest.h>
#include <aws/route53-recovery-control-config/model/UpdateSafetyRuleRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Route53RecoveryControlConfig;
using namespace Aws::Route53RecoveryControlConfig::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* Route53RecoveryControlConfigClient::SERVICE_NAME = "route53-recovery-control-config";
const char* Route53RecoveryControlConfigClient::ALLOCATION_TAG = "Route53RecoveryControlConfigClient";

Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const Route53RecoveryControlConfig::Route53RecoveryControlConfigClientConfiguration& clientConfiguration,
                                                                       std::shared_ptr<Route53RecoveryControlConfigEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const AWSCredentials& credentials,
                                                                       std::shared_ptr<Route53RecoveryControlConfigEndpointProviderBase> endpointProvider,
                                                                       const Route53RecoveryControlConfig::Route53RecoveryControlConfigClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                                       std::shared_ptr<Route53RecoveryControlConfigEndpointProviderBase> endpointProvider,
                                                                       const Route53RecoveryControlConfig::Route53RecoveryControlConfigClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<Route53RecoveryControlConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const AWSCredentials& credentials,
                                                                       const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<Route53RecoveryControlConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

Route53RecoveryControlConfigClient::Route53RecoveryControlConfigClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                                       const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<Route53RecoveryControlConfigErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<Route53RecoveryControlConfigEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
Route53RecoveryControlConfigClient::~Route53RecoveryControlConfigClient()
{
}

std::shared_ptr<Route53RecoveryControlConfigEndpointProviderBase>& Route53RecoveryControlConfigClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void Route53RecoveryControlConfigClient::init(const Route53RecoveryControlConfig::Route53RecoveryControlConfigClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Route53 Recovery Control Config");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void Route53RecoveryControlConfigClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateClusterOutcome Route53RecoveryControlConfigClient::CreateCluster(const CreateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster");
  return CreateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateClusterOutcomeCallable Route53RecoveryControlConfigClient::CreateClusterCallable(const CreateClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::CreateCluster, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::CreateClusterAsync(const CreateClusterRequest& request, const CreateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::CreateCluster, this, request, handler, context, m_executor.get());
}

CreateControlPanelOutcome Route53RecoveryControlConfigClient::CreateControlPanel(const CreateControlPanelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel");
  return CreateControlPanelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateControlPanelOutcomeCallable Route53RecoveryControlConfigClient::CreateControlPanelCallable(const CreateControlPanelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::CreateControlPanel, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::CreateControlPanelAsync(const CreateControlPanelRequest& request, const CreateControlPanelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::CreateControlPanel, this, request, handler, context, m_executor.get());
}

CreateRoutingControlOutcome Route53RecoveryControlConfigClient::CreateRoutingControl(const CreateRoutingControlRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrol");
  return CreateRoutingControlOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRoutingControlOutcomeCallable Route53RecoveryControlConfigClient::CreateRoutingControlCallable(const CreateRoutingControlRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::CreateRoutingControl, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::CreateRoutingControlAsync(const CreateRoutingControlRequest& request, const CreateRoutingControlResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::CreateRoutingControl, this, request, handler, context, m_executor.get());
}

CreateSafetyRuleOutcome Route53RecoveryControlConfigClient::CreateSafetyRule(const CreateSafetyRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/safetyrule");
  return CreateSafetyRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSafetyRuleOutcomeCallable Route53RecoveryControlConfigClient::CreateSafetyRuleCallable(const CreateSafetyRuleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::CreateSafetyRule, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::CreateSafetyRuleAsync(const CreateSafetyRuleRequest& request, const CreateSafetyRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::CreateSafetyRule, this, request, handler, context, m_executor.get());
}

DeleteClusterOutcome Route53RecoveryControlConfigClient::DeleteCluster(const DeleteClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCluster", "Required field: ClusterArn, is not set");
    return DeleteClusterOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterArn());
  return DeleteClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteClusterOutcomeCallable Route53RecoveryControlConfigClient::DeleteClusterCallable(const DeleteClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DeleteCluster, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DeleteClusterAsync(const DeleteClusterRequest& request, const DeleteClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DeleteCluster, this, request, handler, context, m_executor.get());
}

DeleteControlPanelOutcome Route53RecoveryControlConfigClient::DeleteControlPanel(const DeleteControlPanelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ControlPanelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteControlPanel", "Required field: ControlPanelArn, is not set");
    return DeleteControlPanelOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ControlPanelArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetControlPanelArn());
  return DeleteControlPanelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteControlPanelOutcomeCallable Route53RecoveryControlConfigClient::DeleteControlPanelCallable(const DeleteControlPanelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DeleteControlPanel, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DeleteControlPanelAsync(const DeleteControlPanelRequest& request, const DeleteControlPanelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DeleteControlPanel, this, request, handler, context, m_executor.get());
}

DeleteRoutingControlOutcome Route53RecoveryControlConfigClient::DeleteRoutingControl(const DeleteRoutingControlRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RoutingControlArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteRoutingControl", "Required field: RoutingControlArn, is not set");
    return DeleteRoutingControlOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RoutingControlArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrol/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRoutingControlArn());
  return DeleteRoutingControlOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteRoutingControlOutcomeCallable Route53RecoveryControlConfigClient::DeleteRoutingControlCallable(const DeleteRoutingControlRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DeleteRoutingControl, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DeleteRoutingControlAsync(const DeleteRoutingControlRequest& request, const DeleteRoutingControlResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DeleteRoutingControl, this, request, handler, context, m_executor.get());
}

DeleteSafetyRuleOutcome Route53RecoveryControlConfigClient::DeleteSafetyRule(const DeleteSafetyRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SafetyRuleArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSafetyRule", "Required field: SafetyRuleArn, is not set");
    return DeleteSafetyRuleOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SafetyRuleArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/safetyrule/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSafetyRuleArn());
  return DeleteSafetyRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteSafetyRuleOutcomeCallable Route53RecoveryControlConfigClient::DeleteSafetyRuleCallable(const DeleteSafetyRuleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DeleteSafetyRule, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DeleteSafetyRuleAsync(const DeleteSafetyRuleRequest& request, const DeleteSafetyRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DeleteSafetyRule, this, request, handler, context, m_executor.get());
}

DescribeClusterOutcome Route53RecoveryControlConfigClient::DescribeCluster(const DescribeClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeCluster", "Required field: ClusterArn, is not set");
    return DescribeClusterOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterArn());
  return DescribeClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeClusterOutcomeCallable Route53RecoveryControlConfigClient::DescribeClusterCallable(const DescribeClusterRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DescribeCluster, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DescribeClusterAsync(const DescribeClusterRequest& request, const DescribeClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DescribeCluster, this, request, handler, context, m_executor.get());
}

DescribeControlPanelOutcome Route53RecoveryControlConfigClient::DescribeControlPanel(const DescribeControlPanelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ControlPanelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeControlPanel", "Required field: ControlPanelArn, is not set");
    return DescribeControlPanelOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ControlPanelArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetControlPanelArn());
  return DescribeControlPanelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeControlPanelOutcomeCallable Route53RecoveryControlConfigClient::DescribeControlPanelCallable(const DescribeControlPanelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DescribeControlPanel, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DescribeControlPanelAsync(const DescribeControlPanelRequest& request, const DescribeControlPanelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DescribeControlPanel, this, request, handler, context, m_executor.get());
}

DescribeRoutingControlOutcome Route53RecoveryControlConfigClient::DescribeRoutingControl(const DescribeRoutingControlRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RoutingControlArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeRoutingControl", "Required field: RoutingControlArn, is not set");
    return DescribeRoutingControlOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RoutingControlArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrol/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRoutingControlArn());
  return DescribeRoutingControlOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeRoutingControlOutcomeCallable Route53RecoveryControlConfigClient::DescribeRoutingControlCallable(const DescribeRoutingControlRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DescribeRoutingControl, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DescribeRoutingControlAsync(const DescribeRoutingControlRequest& request, const DescribeRoutingControlResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DescribeRoutingControl, this, request, handler, context, m_executor.get());
}

DescribeSafetyRuleOutcome Route53RecoveryControlConfigClient::DescribeSafetyRule(const DescribeSafetyRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.SafetyRuleArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeSafetyRule", "Required field: SafetyRuleArn, is not set");
    return DescribeSafetyRuleOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SafetyRuleArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/safetyrule/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSafetyRuleArn());
  return DescribeSafetyRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeSafetyRuleOutcomeCallable Route53RecoveryControlConfigClient::DescribeSafetyRuleCallable(const DescribeSafetyRuleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::DescribeSafetyRule, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::DescribeSafetyRuleAsync(const DescribeSafetyRuleRequest& request, const DescribeSafetyRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::DescribeSafetyRule, this, request, handler, context, m_executor.get());
}

ListAssociatedRoute53HealthChecksOutcome Route53RecoveryControlConfigClient::ListAssociatedRoute53HealthChecks(const ListAssociatedRoute53HealthChecksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAssociatedRoute53HealthChecks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.RoutingControlArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAssociatedRoute53HealthChecks", "Required field: RoutingControlArn, is not set");
    return ListAssociatedRoute53HealthChecksOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RoutingControlArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAssociatedRoute53HealthChecks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrol/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRoutingControlArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/associatedRoute53HealthChecks");
  return ListAssociatedRoute53HealthChecksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAssociatedRoute53HealthChecksOutcomeCallable Route53RecoveryControlConfigClient::ListAssociatedRoute53HealthChecksCallable(const ListAssociatedRoute53HealthChecksRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListAssociatedRoute53HealthChecks, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListAssociatedRoute53HealthChecksAsync(const ListAssociatedRoute53HealthChecksRequest& request, const ListAssociatedRoute53HealthChecksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListAssociatedRoute53HealthChecks, this, request, handler, context, m_executor.get());
}

ListClustersOutcome Route53RecoveryControlConfigClient::ListClusters(const ListClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster");
  return ListClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListClustersOutcomeCallable Route53RecoveryControlConfigClient::ListClustersCallable(const ListClustersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListClusters, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListClustersAsync(const ListClustersRequest& request, const ListClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListClusters, this, request, handler, context, m_executor.get());
}

ListControlPanelsOutcome Route53RecoveryControlConfigClient::ListControlPanels(const ListControlPanelsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListControlPanels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListControlPanels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanels");
  return ListControlPanelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListControlPanelsOutcomeCallable Route53RecoveryControlConfigClient::ListControlPanelsCallable(const ListControlPanelsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListControlPanels, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListControlPanelsAsync(const ListControlPanelsRequest& request, const ListControlPanelsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListControlPanels, this, request, handler, context, m_executor.get());
}

ListRoutingControlsOutcome Route53RecoveryControlConfigClient::ListRoutingControls(const ListRoutingControlsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRoutingControls, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ControlPanelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListRoutingControls", "Required field: ControlPanelArn, is not set");
    return ListRoutingControlsOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ControlPanelArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRoutingControls, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetControlPanelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrols");
  return ListRoutingControlsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListRoutingControlsOutcomeCallable Route53RecoveryControlConfigClient::ListRoutingControlsCallable(const ListRoutingControlsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListRoutingControls, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListRoutingControlsAsync(const ListRoutingControlsRequest& request, const ListRoutingControlsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListRoutingControls, this, request, handler, context, m_executor.get());
}

ListSafetyRulesOutcome Route53RecoveryControlConfigClient::ListSafetyRules(const ListSafetyRulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSafetyRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ControlPanelArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListSafetyRules", "Required field: ControlPanelArn, is not set");
    return ListSafetyRulesOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ControlPanelArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSafetyRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetControlPanelArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/safetyrules");
  return ListSafetyRulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListSafetyRulesOutcomeCallable Route53RecoveryControlConfigClient::ListSafetyRulesCallable(const ListSafetyRulesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListSafetyRules, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListSafetyRulesAsync(const ListSafetyRulesRequest& request, const ListSafetyRulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListSafetyRules, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome Route53RecoveryControlConfigClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable Route53RecoveryControlConfigClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::ListTagsForResource, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

TagResourceOutcome Route53RecoveryControlConfigClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable Route53RecoveryControlConfigClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::TagResource, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome Route53RecoveryControlConfigClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<Route53RecoveryControlConfigErrors>(Route53RecoveryControlConfigErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable Route53RecoveryControlConfigClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::UntagResource, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateControlPanelOutcome Route53RecoveryControlConfigClient::UpdateControlPanel(const UpdateControlPanelRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateControlPanel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/controlpanel");
  return UpdateControlPanelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateControlPanelOutcomeCallable Route53RecoveryControlConfigClient::UpdateControlPanelCallable(const UpdateControlPanelRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::UpdateControlPanel, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::UpdateControlPanelAsync(const UpdateControlPanelRequest& request, const UpdateControlPanelResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::UpdateControlPanel, this, request, handler, context, m_executor.get());
}

UpdateRoutingControlOutcome Route53RecoveryControlConfigClient::UpdateRoutingControl(const UpdateRoutingControlRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRoutingControl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/routingcontrol");
  return UpdateRoutingControlOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateRoutingControlOutcomeCallable Route53RecoveryControlConfigClient::UpdateRoutingControlCallable(const UpdateRoutingControlRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::UpdateRoutingControl, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::UpdateRoutingControlAsync(const UpdateRoutingControlRequest& request, const UpdateRoutingControlResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::UpdateRoutingControl, this, request, handler, context, m_executor.get());
}

UpdateSafetyRuleOutcome Route53RecoveryControlConfigClient::UpdateSafetyRule(const UpdateSafetyRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSafetyRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/safetyrule");
  return UpdateSafetyRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

UpdateSafetyRuleOutcomeCallable Route53RecoveryControlConfigClient::UpdateSafetyRuleCallable(const UpdateSafetyRuleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &Route53RecoveryControlConfigClient::UpdateSafetyRule, this, request, m_executor.get());
}

void Route53RecoveryControlConfigClient::UpdateSafetyRuleAsync(const UpdateSafetyRuleRequest& request, const UpdateSafetyRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&Route53RecoveryControlConfigClient::UpdateSafetyRule, this, request, handler, context, m_executor.get());
}

