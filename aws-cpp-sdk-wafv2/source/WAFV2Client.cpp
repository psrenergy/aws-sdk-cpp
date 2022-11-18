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

#include <aws/wafv2/WAFV2Client.h>
#include <aws/wafv2/WAFV2ErrorMarshaller.h>
#include <aws/wafv2/WAFV2EndpointProvider.h>
#include <aws/wafv2/model/AssociateWebACLRequest.h>
#include <aws/wafv2/model/CheckCapacityRequest.h>
#include <aws/wafv2/model/CreateIPSetRequest.h>
#include <aws/wafv2/model/CreateRegexPatternSetRequest.h>
#include <aws/wafv2/model/CreateRuleGroupRequest.h>
#include <aws/wafv2/model/CreateWebACLRequest.h>
#include <aws/wafv2/model/DeleteFirewallManagerRuleGroupsRequest.h>
#include <aws/wafv2/model/DeleteIPSetRequest.h>
#include <aws/wafv2/model/DeleteLoggingConfigurationRequest.h>
#include <aws/wafv2/model/DeletePermissionPolicyRequest.h>
#include <aws/wafv2/model/DeleteRegexPatternSetRequest.h>
#include <aws/wafv2/model/DeleteRuleGroupRequest.h>
#include <aws/wafv2/model/DeleteWebACLRequest.h>
#include <aws/wafv2/model/DescribeManagedRuleGroupRequest.h>
#include <aws/wafv2/model/DisassociateWebACLRequest.h>
#include <aws/wafv2/model/GenerateMobileSdkReleaseUrlRequest.h>
#include <aws/wafv2/model/GetIPSetRequest.h>
#include <aws/wafv2/model/GetLoggingConfigurationRequest.h>
#include <aws/wafv2/model/GetManagedRuleSetRequest.h>
#include <aws/wafv2/model/GetMobileSdkReleaseRequest.h>
#include <aws/wafv2/model/GetPermissionPolicyRequest.h>
#include <aws/wafv2/model/GetRateBasedStatementManagedKeysRequest.h>
#include <aws/wafv2/model/GetRegexPatternSetRequest.h>
#include <aws/wafv2/model/GetRuleGroupRequest.h>
#include <aws/wafv2/model/GetSampledRequestsRequest.h>
#include <aws/wafv2/model/GetWebACLRequest.h>
#include <aws/wafv2/model/GetWebACLForResourceRequest.h>
#include <aws/wafv2/model/ListAvailableManagedRuleGroupVersionsRequest.h>
#include <aws/wafv2/model/ListAvailableManagedRuleGroupsRequest.h>
#include <aws/wafv2/model/ListIPSetsRequest.h>
#include <aws/wafv2/model/ListLoggingConfigurationsRequest.h>
#include <aws/wafv2/model/ListManagedRuleSetsRequest.h>
#include <aws/wafv2/model/ListMobileSdkReleasesRequest.h>
#include <aws/wafv2/model/ListRegexPatternSetsRequest.h>
#include <aws/wafv2/model/ListResourcesForWebACLRequest.h>
#include <aws/wafv2/model/ListRuleGroupsRequest.h>
#include <aws/wafv2/model/ListTagsForResourceRequest.h>
#include <aws/wafv2/model/ListWebACLsRequest.h>
#include <aws/wafv2/model/PutLoggingConfigurationRequest.h>
#include <aws/wafv2/model/PutManagedRuleSetVersionsRequest.h>
#include <aws/wafv2/model/PutPermissionPolicyRequest.h>
#include <aws/wafv2/model/TagResourceRequest.h>
#include <aws/wafv2/model/UntagResourceRequest.h>
#include <aws/wafv2/model/UpdateIPSetRequest.h>
#include <aws/wafv2/model/UpdateManagedRuleSetVersionExpiryDateRequest.h>
#include <aws/wafv2/model/UpdateRegexPatternSetRequest.h>
#include <aws/wafv2/model/UpdateRuleGroupRequest.h>
#include <aws/wafv2/model/UpdateWebACLRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::WAFV2;
using namespace Aws::WAFV2::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* WAFV2Client::SERVICE_NAME = "wafv2";
const char* WAFV2Client::ALLOCATION_TAG = "WAFV2Client";

WAFV2Client::WAFV2Client(const WAFV2::WAFV2ClientConfiguration& clientConfiguration,
                         std::shared_ptr<WAFV2EndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WAFV2Client::WAFV2Client(const AWSCredentials& credentials,
                         std::shared_ptr<WAFV2EndpointProviderBase> endpointProvider,
                         const WAFV2::WAFV2ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WAFV2Client::WAFV2Client(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         std::shared_ptr<WAFV2EndpointProviderBase> endpointProvider,
                         const WAFV2::WAFV2ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  WAFV2Client::WAFV2Client(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<WAFV2EndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WAFV2Client::WAFV2Client(const AWSCredentials& credentials,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WAFV2EndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WAFV2Client::WAFV2Client(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFV2ErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WAFV2EndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
WAFV2Client::~WAFV2Client()
{
}

std::shared_ptr<WAFV2EndpointProviderBase>& WAFV2Client::accessEndpointProvider()
{
  return m_endpointProvider;
}

void WAFV2Client::init(const WAFV2::WAFV2ClientConfiguration& config)
{
  AWSClient::SetServiceClientName("WAFV2");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void WAFV2Client::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateWebACLOutcome WAFV2Client::AssociateWebACL(const AssociateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateWebACLOutcomeCallable WAFV2Client::AssociateWebACLCallable(const AssociateWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateWebACL, request, m_executor.get());
}

void WAFV2Client::AssociateWebACLAsync(const AssociateWebACLRequest& request, const AssociateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateWebACL, request, handler, context, m_executor.get());
}

CheckCapacityOutcome WAFV2Client::CheckCapacity(const CheckCapacityRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CheckCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CheckCapacity, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CheckCapacityOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CheckCapacityOutcomeCallable WAFV2Client::CheckCapacityCallable(const CheckCapacityRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CheckCapacity, request, m_executor.get());
}

void WAFV2Client::CheckCapacityAsync(const CheckCapacityRequest& request, const CheckCapacityResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CheckCapacity, request, handler, context, m_executor.get());
}

CreateIPSetOutcome WAFV2Client::CreateIPSet(const CreateIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateIPSetOutcomeCallable WAFV2Client::CreateIPSetCallable(const CreateIPSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateIPSet, request, m_executor.get());
}

void WAFV2Client::CreateIPSetAsync(const CreateIPSetRequest& request, const CreateIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateIPSet, request, handler, context, m_executor.get());
}

CreateRegexPatternSetOutcome WAFV2Client::CreateRegexPatternSet(const CreateRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRegexPatternSetOutcomeCallable WAFV2Client::CreateRegexPatternSetCallable(const CreateRegexPatternSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateRegexPatternSet, request, m_executor.get());
}

void WAFV2Client::CreateRegexPatternSetAsync(const CreateRegexPatternSetRequest& request, const CreateRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateRegexPatternSet, request, handler, context, m_executor.get());
}

CreateRuleGroupOutcome WAFV2Client::CreateRuleGroup(const CreateRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRuleGroupOutcomeCallable WAFV2Client::CreateRuleGroupCallable(const CreateRuleGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateRuleGroup, request, m_executor.get());
}

void WAFV2Client::CreateRuleGroupAsync(const CreateRuleGroupRequest& request, const CreateRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateRuleGroup, request, handler, context, m_executor.get());
}

CreateWebACLOutcome WAFV2Client::CreateWebACL(const CreateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWebACLOutcomeCallable WAFV2Client::CreateWebACLCallable(const CreateWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateWebACL, request, m_executor.get());
}

void WAFV2Client::CreateWebACLAsync(const CreateWebACLRequest& request, const CreateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateWebACL, request, handler, context, m_executor.get());
}

DeleteFirewallManagerRuleGroupsOutcome WAFV2Client::DeleteFirewallManagerRuleGroups(const DeleteFirewallManagerRuleGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteFirewallManagerRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteFirewallManagerRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteFirewallManagerRuleGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteFirewallManagerRuleGroupsOutcomeCallable WAFV2Client::DeleteFirewallManagerRuleGroupsCallable(const DeleteFirewallManagerRuleGroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteFirewallManagerRuleGroups, request, m_executor.get());
}

void WAFV2Client::DeleteFirewallManagerRuleGroupsAsync(const DeleteFirewallManagerRuleGroupsRequest& request, const DeleteFirewallManagerRuleGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteFirewallManagerRuleGroups, request, handler, context, m_executor.get());
}

DeleteIPSetOutcome WAFV2Client::DeleteIPSet(const DeleteIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteIPSetOutcomeCallable WAFV2Client::DeleteIPSetCallable(const DeleteIPSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteIPSet, request, m_executor.get());
}

void WAFV2Client::DeleteIPSetAsync(const DeleteIPSetRequest& request, const DeleteIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteIPSet, request, handler, context, m_executor.get());
}

DeleteLoggingConfigurationOutcome WAFV2Client::DeleteLoggingConfiguration(const DeleteLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteLoggingConfigurationOutcomeCallable WAFV2Client::DeleteLoggingConfigurationCallable(const DeleteLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteLoggingConfiguration, request, m_executor.get());
}

void WAFV2Client::DeleteLoggingConfigurationAsync(const DeleteLoggingConfigurationRequest& request, const DeleteLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteLoggingConfiguration, request, handler, context, m_executor.get());
}

DeletePermissionPolicyOutcome WAFV2Client::DeletePermissionPolicy(const DeletePermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePermissionPolicyOutcomeCallable WAFV2Client::DeletePermissionPolicyCallable(const DeletePermissionPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeletePermissionPolicy, request, m_executor.get());
}

void WAFV2Client::DeletePermissionPolicyAsync(const DeletePermissionPolicyRequest& request, const DeletePermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeletePermissionPolicy, request, handler, context, m_executor.get());
}

DeleteRegexPatternSetOutcome WAFV2Client::DeleteRegexPatternSet(const DeleteRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRegexPatternSetOutcomeCallable WAFV2Client::DeleteRegexPatternSetCallable(const DeleteRegexPatternSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteRegexPatternSet, request, m_executor.get());
}

void WAFV2Client::DeleteRegexPatternSetAsync(const DeleteRegexPatternSetRequest& request, const DeleteRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteRegexPatternSet, request, handler, context, m_executor.get());
}

DeleteRuleGroupOutcome WAFV2Client::DeleteRuleGroup(const DeleteRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRuleGroupOutcomeCallable WAFV2Client::DeleteRuleGroupCallable(const DeleteRuleGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteRuleGroup, request, m_executor.get());
}

void WAFV2Client::DeleteRuleGroupAsync(const DeleteRuleGroupRequest& request, const DeleteRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteRuleGroup, request, handler, context, m_executor.get());
}

DeleteWebACLOutcome WAFV2Client::DeleteWebACL(const DeleteWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteWebACLOutcomeCallable WAFV2Client::DeleteWebACLCallable(const DeleteWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteWebACL, request, m_executor.get());
}

void WAFV2Client::DeleteWebACLAsync(const DeleteWebACLRequest& request, const DeleteWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteWebACL, request, handler, context, m_executor.get());
}

DescribeManagedRuleGroupOutcome WAFV2Client::DescribeManagedRuleGroup(const DescribeManagedRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeManagedRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeManagedRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeManagedRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeManagedRuleGroupOutcomeCallable WAFV2Client::DescribeManagedRuleGroupCallable(const DescribeManagedRuleGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeManagedRuleGroup, request, m_executor.get());
}

void WAFV2Client::DescribeManagedRuleGroupAsync(const DescribeManagedRuleGroupRequest& request, const DescribeManagedRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeManagedRuleGroup, request, handler, context, m_executor.get());
}

DisassociateWebACLOutcome WAFV2Client::DisassociateWebACL(const DisassociateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateWebACLOutcomeCallable WAFV2Client::DisassociateWebACLCallable(const DisassociateWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateWebACL, request, m_executor.get());
}

void WAFV2Client::DisassociateWebACLAsync(const DisassociateWebACLRequest& request, const DisassociateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateWebACL, request, handler, context, m_executor.get());
}

GenerateMobileSdkReleaseUrlOutcome WAFV2Client::GenerateMobileSdkReleaseUrl(const GenerateMobileSdkReleaseUrlRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GenerateMobileSdkReleaseUrl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GenerateMobileSdkReleaseUrl, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GenerateMobileSdkReleaseUrlOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GenerateMobileSdkReleaseUrlOutcomeCallable WAFV2Client::GenerateMobileSdkReleaseUrlCallable(const GenerateMobileSdkReleaseUrlRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GenerateMobileSdkReleaseUrl, request, m_executor.get());
}

void WAFV2Client::GenerateMobileSdkReleaseUrlAsync(const GenerateMobileSdkReleaseUrlRequest& request, const GenerateMobileSdkReleaseUrlResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GenerateMobileSdkReleaseUrl, request, handler, context, m_executor.get());
}

GetIPSetOutcome WAFV2Client::GetIPSet(const GetIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetIPSetOutcomeCallable WAFV2Client::GetIPSetCallable(const GetIPSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetIPSet, request, m_executor.get());
}

void WAFV2Client::GetIPSetAsync(const GetIPSetRequest& request, const GetIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetIPSet, request, handler, context, m_executor.get());
}

GetLoggingConfigurationOutcome WAFV2Client::GetLoggingConfiguration(const GetLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetLoggingConfigurationOutcomeCallable WAFV2Client::GetLoggingConfigurationCallable(const GetLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetLoggingConfiguration, request, m_executor.get());
}

void WAFV2Client::GetLoggingConfigurationAsync(const GetLoggingConfigurationRequest& request, const GetLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetLoggingConfiguration, request, handler, context, m_executor.get());
}

GetManagedRuleSetOutcome WAFV2Client::GetManagedRuleSet(const GetManagedRuleSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetManagedRuleSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetManagedRuleSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetManagedRuleSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetManagedRuleSetOutcomeCallable WAFV2Client::GetManagedRuleSetCallable(const GetManagedRuleSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetManagedRuleSet, request, m_executor.get());
}

void WAFV2Client::GetManagedRuleSetAsync(const GetManagedRuleSetRequest& request, const GetManagedRuleSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetManagedRuleSet, request, handler, context, m_executor.get());
}

GetMobileSdkReleaseOutcome WAFV2Client::GetMobileSdkRelease(const GetMobileSdkReleaseRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMobileSdkRelease, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMobileSdkRelease, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetMobileSdkReleaseOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetMobileSdkReleaseOutcomeCallable WAFV2Client::GetMobileSdkReleaseCallable(const GetMobileSdkReleaseRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetMobileSdkRelease, request, m_executor.get());
}

void WAFV2Client::GetMobileSdkReleaseAsync(const GetMobileSdkReleaseRequest& request, const GetMobileSdkReleaseResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetMobileSdkRelease, request, handler, context, m_executor.get());
}

GetPermissionPolicyOutcome WAFV2Client::GetPermissionPolicy(const GetPermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetPermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetPermissionPolicyOutcomeCallable WAFV2Client::GetPermissionPolicyCallable(const GetPermissionPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetPermissionPolicy, request, m_executor.get());
}

void WAFV2Client::GetPermissionPolicyAsync(const GetPermissionPolicyRequest& request, const GetPermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetPermissionPolicy, request, handler, context, m_executor.get());
}

GetRateBasedStatementManagedKeysOutcome WAFV2Client::GetRateBasedStatementManagedKeys(const GetRateBasedStatementManagedKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRateBasedStatementManagedKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRateBasedStatementManagedKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRateBasedStatementManagedKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRateBasedStatementManagedKeysOutcomeCallable WAFV2Client::GetRateBasedStatementManagedKeysCallable(const GetRateBasedStatementManagedKeysRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRateBasedStatementManagedKeys, request, m_executor.get());
}

void WAFV2Client::GetRateBasedStatementManagedKeysAsync(const GetRateBasedStatementManagedKeysRequest& request, const GetRateBasedStatementManagedKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRateBasedStatementManagedKeys, request, handler, context, m_executor.get());
}

GetRegexPatternSetOutcome WAFV2Client::GetRegexPatternSet(const GetRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRegexPatternSetOutcomeCallable WAFV2Client::GetRegexPatternSetCallable(const GetRegexPatternSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRegexPatternSet, request, m_executor.get());
}

void WAFV2Client::GetRegexPatternSetAsync(const GetRegexPatternSetRequest& request, const GetRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRegexPatternSet, request, handler, context, m_executor.get());
}

GetRuleGroupOutcome WAFV2Client::GetRuleGroup(const GetRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRuleGroupOutcomeCallable WAFV2Client::GetRuleGroupCallable(const GetRuleGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRuleGroup, request, m_executor.get());
}

void WAFV2Client::GetRuleGroupAsync(const GetRuleGroupRequest& request, const GetRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRuleGroup, request, handler, context, m_executor.get());
}

GetSampledRequestsOutcome WAFV2Client::GetSampledRequests(const GetSampledRequestsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSampledRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSampledRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSampledRequestsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSampledRequestsOutcomeCallable WAFV2Client::GetSampledRequestsCallable(const GetSampledRequestsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSampledRequests, request, m_executor.get());
}

void WAFV2Client::GetSampledRequestsAsync(const GetSampledRequestsRequest& request, const GetSampledRequestsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSampledRequests, request, handler, context, m_executor.get());
}

GetWebACLOutcome WAFV2Client::GetWebACL(const GetWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetWebACLOutcomeCallable WAFV2Client::GetWebACLCallable(const GetWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetWebACL, request, m_executor.get());
}

void WAFV2Client::GetWebACLAsync(const GetWebACLRequest& request, const GetWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetWebACL, request, handler, context, m_executor.get());
}

GetWebACLForResourceOutcome WAFV2Client::GetWebACLForResource(const GetWebACLForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetWebACLForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetWebACLForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetWebACLForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetWebACLForResourceOutcomeCallable WAFV2Client::GetWebACLForResourceCallable(const GetWebACLForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetWebACLForResource, request, m_executor.get());
}

void WAFV2Client::GetWebACLForResourceAsync(const GetWebACLForResourceRequest& request, const GetWebACLForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetWebACLForResource, request, handler, context, m_executor.get());
}

ListAvailableManagedRuleGroupVersionsOutcome WAFV2Client::ListAvailableManagedRuleGroupVersions(const ListAvailableManagedRuleGroupVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAvailableManagedRuleGroupVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAvailableManagedRuleGroupVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAvailableManagedRuleGroupVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAvailableManagedRuleGroupVersionsOutcomeCallable WAFV2Client::ListAvailableManagedRuleGroupVersionsCallable(const ListAvailableManagedRuleGroupVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAvailableManagedRuleGroupVersions, request, m_executor.get());
}

void WAFV2Client::ListAvailableManagedRuleGroupVersionsAsync(const ListAvailableManagedRuleGroupVersionsRequest& request, const ListAvailableManagedRuleGroupVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAvailableManagedRuleGroupVersions, request, handler, context, m_executor.get());
}

ListAvailableManagedRuleGroupsOutcome WAFV2Client::ListAvailableManagedRuleGroups(const ListAvailableManagedRuleGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAvailableManagedRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAvailableManagedRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAvailableManagedRuleGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAvailableManagedRuleGroupsOutcomeCallable WAFV2Client::ListAvailableManagedRuleGroupsCallable(const ListAvailableManagedRuleGroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAvailableManagedRuleGroups, request, m_executor.get());
}

void WAFV2Client::ListAvailableManagedRuleGroupsAsync(const ListAvailableManagedRuleGroupsRequest& request, const ListAvailableManagedRuleGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAvailableManagedRuleGroups, request, handler, context, m_executor.get());
}

ListIPSetsOutcome WAFV2Client::ListIPSets(const ListIPSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIPSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIPSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListIPSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListIPSetsOutcomeCallable WAFV2Client::ListIPSetsCallable(const ListIPSetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListIPSets, request, m_executor.get());
}

void WAFV2Client::ListIPSetsAsync(const ListIPSetsRequest& request, const ListIPSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListIPSets, request, handler, context, m_executor.get());
}

ListLoggingConfigurationsOutcome WAFV2Client::ListLoggingConfigurations(const ListLoggingConfigurationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListLoggingConfigurationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListLoggingConfigurationsOutcomeCallable WAFV2Client::ListLoggingConfigurationsCallable(const ListLoggingConfigurationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListLoggingConfigurations, request, m_executor.get());
}

void WAFV2Client::ListLoggingConfigurationsAsync(const ListLoggingConfigurationsRequest& request, const ListLoggingConfigurationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListLoggingConfigurations, request, handler, context, m_executor.get());
}

ListManagedRuleSetsOutcome WAFV2Client::ListManagedRuleSets(const ListManagedRuleSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListManagedRuleSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListManagedRuleSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListManagedRuleSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListManagedRuleSetsOutcomeCallable WAFV2Client::ListManagedRuleSetsCallable(const ListManagedRuleSetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListManagedRuleSets, request, m_executor.get());
}

void WAFV2Client::ListManagedRuleSetsAsync(const ListManagedRuleSetsRequest& request, const ListManagedRuleSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListManagedRuleSets, request, handler, context, m_executor.get());
}

ListMobileSdkReleasesOutcome WAFV2Client::ListMobileSdkReleases(const ListMobileSdkReleasesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMobileSdkReleases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMobileSdkReleases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListMobileSdkReleasesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListMobileSdkReleasesOutcomeCallable WAFV2Client::ListMobileSdkReleasesCallable(const ListMobileSdkReleasesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListMobileSdkReleases, request, m_executor.get());
}

void WAFV2Client::ListMobileSdkReleasesAsync(const ListMobileSdkReleasesRequest& request, const ListMobileSdkReleasesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListMobileSdkReleases, request, handler, context, m_executor.get());
}

ListRegexPatternSetsOutcome WAFV2Client::ListRegexPatternSets(const ListRegexPatternSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRegexPatternSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRegexPatternSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRegexPatternSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRegexPatternSetsOutcomeCallable WAFV2Client::ListRegexPatternSetsCallable(const ListRegexPatternSetsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListRegexPatternSets, request, m_executor.get());
}

void WAFV2Client::ListRegexPatternSetsAsync(const ListRegexPatternSetsRequest& request, const ListRegexPatternSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListRegexPatternSets, request, handler, context, m_executor.get());
}

ListResourcesForWebACLOutcome WAFV2Client::ListResourcesForWebACL(const ListResourcesForWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResourcesForWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResourcesForWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListResourcesForWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListResourcesForWebACLOutcomeCallable WAFV2Client::ListResourcesForWebACLCallable(const ListResourcesForWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListResourcesForWebACL, request, m_executor.get());
}

void WAFV2Client::ListResourcesForWebACLAsync(const ListResourcesForWebACLRequest& request, const ListResourcesForWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListResourcesForWebACL, request, handler, context, m_executor.get());
}

ListRuleGroupsOutcome WAFV2Client::ListRuleGroups(const ListRuleGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRuleGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRuleGroupsOutcomeCallable WAFV2Client::ListRuleGroupsCallable(const ListRuleGroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListRuleGroups, request, m_executor.get());
}

void WAFV2Client::ListRuleGroupsAsync(const ListRuleGroupsRequest& request, const ListRuleGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListRuleGroups, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome WAFV2Client::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable WAFV2Client::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void WAFV2Client::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

ListWebACLsOutcome WAFV2Client::ListWebACLs(const ListWebACLsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWebACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWebACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListWebACLsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWebACLsOutcomeCallable WAFV2Client::ListWebACLsCallable(const ListWebACLsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListWebACLs, request, m_executor.get());
}

void WAFV2Client::ListWebACLsAsync(const ListWebACLsRequest& request, const ListWebACLsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListWebACLs, request, handler, context, m_executor.get());
}

PutLoggingConfigurationOutcome WAFV2Client::PutLoggingConfiguration(const PutLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutLoggingConfigurationOutcomeCallable WAFV2Client::PutLoggingConfigurationCallable(const PutLoggingConfigurationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutLoggingConfiguration, request, m_executor.get());
}

void WAFV2Client::PutLoggingConfigurationAsync(const PutLoggingConfigurationRequest& request, const PutLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutLoggingConfiguration, request, handler, context, m_executor.get());
}

PutManagedRuleSetVersionsOutcome WAFV2Client::PutManagedRuleSetVersions(const PutManagedRuleSetVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutManagedRuleSetVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutManagedRuleSetVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutManagedRuleSetVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutManagedRuleSetVersionsOutcomeCallable WAFV2Client::PutManagedRuleSetVersionsCallable(const PutManagedRuleSetVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutManagedRuleSetVersions, request, m_executor.get());
}

void WAFV2Client::PutManagedRuleSetVersionsAsync(const PutManagedRuleSetVersionsRequest& request, const PutManagedRuleSetVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutManagedRuleSetVersions, request, handler, context, m_executor.get());
}

PutPermissionPolicyOutcome WAFV2Client::PutPermissionPolicy(const PutPermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutPermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutPermissionPolicyOutcomeCallable WAFV2Client::PutPermissionPolicyCallable(const PutPermissionPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutPermissionPolicy, request, m_executor.get());
}

void WAFV2Client::PutPermissionPolicyAsync(const PutPermissionPolicyRequest& request, const PutPermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutPermissionPolicy, request, handler, context, m_executor.get());
}

TagResourceOutcome WAFV2Client::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable WAFV2Client::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void WAFV2Client::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome WAFV2Client::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable WAFV2Client::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void WAFV2Client::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateIPSetOutcome WAFV2Client::UpdateIPSet(const UpdateIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateIPSetOutcomeCallable WAFV2Client::UpdateIPSetCallable(const UpdateIPSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateIPSet, request, m_executor.get());
}

void WAFV2Client::UpdateIPSetAsync(const UpdateIPSetRequest& request, const UpdateIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateIPSet, request, handler, context, m_executor.get());
}

UpdateManagedRuleSetVersionExpiryDateOutcome WAFV2Client::UpdateManagedRuleSetVersionExpiryDate(const UpdateManagedRuleSetVersionExpiryDateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateManagedRuleSetVersionExpiryDate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateManagedRuleSetVersionExpiryDate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateManagedRuleSetVersionExpiryDateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateManagedRuleSetVersionExpiryDateOutcomeCallable WAFV2Client::UpdateManagedRuleSetVersionExpiryDateCallable(const UpdateManagedRuleSetVersionExpiryDateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateManagedRuleSetVersionExpiryDate, request, m_executor.get());
}

void WAFV2Client::UpdateManagedRuleSetVersionExpiryDateAsync(const UpdateManagedRuleSetVersionExpiryDateRequest& request, const UpdateManagedRuleSetVersionExpiryDateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateManagedRuleSetVersionExpiryDate, request, handler, context, m_executor.get());
}

UpdateRegexPatternSetOutcome WAFV2Client::UpdateRegexPatternSet(const UpdateRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRegexPatternSetOutcomeCallable WAFV2Client::UpdateRegexPatternSetCallable(const UpdateRegexPatternSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateRegexPatternSet, request, m_executor.get());
}

void WAFV2Client::UpdateRegexPatternSetAsync(const UpdateRegexPatternSetRequest& request, const UpdateRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateRegexPatternSet, request, handler, context, m_executor.get());
}

UpdateRuleGroupOutcome WAFV2Client::UpdateRuleGroup(const UpdateRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRuleGroupOutcomeCallable WAFV2Client::UpdateRuleGroupCallable(const UpdateRuleGroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateRuleGroup, request, m_executor.get());
}

void WAFV2Client::UpdateRuleGroupAsync(const UpdateRuleGroupRequest& request, const UpdateRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateRuleGroup, request, handler, context, m_executor.get());
}

UpdateWebACLOutcome WAFV2Client::UpdateWebACL(const UpdateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateWebACLOutcomeCallable WAFV2Client::UpdateWebACLCallable(const UpdateWebACLRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateWebACL, request, m_executor.get());
}

void WAFV2Client::UpdateWebACLAsync(const UpdateWebACLRequest& request, const UpdateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateWebACL, request, handler, context, m_executor.get());
}

