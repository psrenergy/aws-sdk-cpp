﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
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

#include <aws/waf/WAFClient.h>
#include <aws/waf/WAFErrorMarshaller.h>
#include <aws/waf/WAFEndpointProvider.h>
#include <aws/waf/model/CreateByteMatchSetRequest.h>
#include <aws/waf/model/CreateGeoMatchSetRequest.h>
#include <aws/waf/model/CreateIPSetRequest.h>
#include <aws/waf/model/CreateRateBasedRuleRequest.h>
#include <aws/waf/model/CreateRegexMatchSetRequest.h>
#include <aws/waf/model/CreateRegexPatternSetRequest.h>
#include <aws/waf/model/CreateRuleRequest.h>
#include <aws/waf/model/CreateRuleGroupRequest.h>
#include <aws/waf/model/CreateSizeConstraintSetRequest.h>
#include <aws/waf/model/CreateSqlInjectionMatchSetRequest.h>
#include <aws/waf/model/CreateWebACLRequest.h>
#include <aws/waf/model/CreateWebACLMigrationStackRequest.h>
#include <aws/waf/model/CreateXssMatchSetRequest.h>
#include <aws/waf/model/DeleteByteMatchSetRequest.h>
#include <aws/waf/model/DeleteGeoMatchSetRequest.h>
#include <aws/waf/model/DeleteIPSetRequest.h>
#include <aws/waf/model/DeleteLoggingConfigurationRequest.h>
#include <aws/waf/model/DeletePermissionPolicyRequest.h>
#include <aws/waf/model/DeleteRateBasedRuleRequest.h>
#include <aws/waf/model/DeleteRegexMatchSetRequest.h>
#include <aws/waf/model/DeleteRegexPatternSetRequest.h>
#include <aws/waf/model/DeleteRuleRequest.h>
#include <aws/waf/model/DeleteRuleGroupRequest.h>
#include <aws/waf/model/DeleteSizeConstraintSetRequest.h>
#include <aws/waf/model/DeleteSqlInjectionMatchSetRequest.h>
#include <aws/waf/model/DeleteWebACLRequest.h>
#include <aws/waf/model/DeleteXssMatchSetRequest.h>
#include <aws/waf/model/GetByteMatchSetRequest.h>
#include <aws/waf/model/GetChangeTokenRequest.h>
#include <aws/waf/model/GetChangeTokenStatusRequest.h>
#include <aws/waf/model/GetGeoMatchSetRequest.h>
#include <aws/waf/model/GetIPSetRequest.h>
#include <aws/waf/model/GetLoggingConfigurationRequest.h>
#include <aws/waf/model/GetPermissionPolicyRequest.h>
#include <aws/waf/model/GetRateBasedRuleRequest.h>
#include <aws/waf/model/GetRateBasedRuleManagedKeysRequest.h>
#include <aws/waf/model/GetRegexMatchSetRequest.h>
#include <aws/waf/model/GetRegexPatternSetRequest.h>
#include <aws/waf/model/GetRuleRequest.h>
#include <aws/waf/model/GetRuleGroupRequest.h>
#include <aws/waf/model/GetSampledRequestsRequest.h>
#include <aws/waf/model/GetSizeConstraintSetRequest.h>
#include <aws/waf/model/GetSqlInjectionMatchSetRequest.h>
#include <aws/waf/model/GetWebACLRequest.h>
#include <aws/waf/model/GetXssMatchSetRequest.h>
#include <aws/waf/model/ListActivatedRulesInRuleGroupRequest.h>
#include <aws/waf/model/ListByteMatchSetsRequest.h>
#include <aws/waf/model/ListGeoMatchSetsRequest.h>
#include <aws/waf/model/ListIPSetsRequest.h>
#include <aws/waf/model/ListLoggingConfigurationsRequest.h>
#include <aws/waf/model/ListRateBasedRulesRequest.h>
#include <aws/waf/model/ListRegexMatchSetsRequest.h>
#include <aws/waf/model/ListRegexPatternSetsRequest.h>
#include <aws/waf/model/ListRuleGroupsRequest.h>
#include <aws/waf/model/ListRulesRequest.h>
#include <aws/waf/model/ListSizeConstraintSetsRequest.h>
#include <aws/waf/model/ListSqlInjectionMatchSetsRequest.h>
#include <aws/waf/model/ListSubscribedRuleGroupsRequest.h>
#include <aws/waf/model/ListTagsForResourceRequest.h>
#include <aws/waf/model/ListWebACLsRequest.h>
#include <aws/waf/model/ListXssMatchSetsRequest.h>
#include <aws/waf/model/PutLoggingConfigurationRequest.h>
#include <aws/waf/model/PutPermissionPolicyRequest.h>
#include <aws/waf/model/TagResourceRequest.h>
#include <aws/waf/model/UntagResourceRequest.h>
#include <aws/waf/model/UpdateByteMatchSetRequest.h>
#include <aws/waf/model/UpdateGeoMatchSetRequest.h>
#include <aws/waf/model/UpdateIPSetRequest.h>
#include <aws/waf/model/UpdateRateBasedRuleRequest.h>
#include <aws/waf/model/UpdateRegexMatchSetRequest.h>
#include <aws/waf/model/UpdateRegexPatternSetRequest.h>
#include <aws/waf/model/UpdateRuleRequest.h>
#include <aws/waf/model/UpdateRuleGroupRequest.h>
#include <aws/waf/model/UpdateSizeConstraintSetRequest.h>
#include <aws/waf/model/UpdateSqlInjectionMatchSetRequest.h>
#include <aws/waf/model/UpdateWebACLRequest.h>
#include <aws/waf/model/UpdateXssMatchSetRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::WAF;
using namespace Aws::WAF::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* WAFClient::SERVICE_NAME = "waf";
const char* WAFClient::ALLOCATION_TAG = "WAFClient";

WAFClient::WAFClient(const WAF::WAFClientConfiguration& clientConfiguration,
                     std::shared_ptr<WAFEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WAFClient::WAFClient(const AWSCredentials& credentials,
                     std::shared_ptr<WAFEndpointProviderBase> endpointProvider,
                     const WAF::WAFClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WAFClient::WAFClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     std::shared_ptr<WAFEndpointProviderBase> endpointProvider,
                     const WAF::WAFClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  WAFClient::WAFClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<WAFEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WAFClient::WAFClient(const AWSCredentials& credentials,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WAFEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WAFClient::WAFClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WAFErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WAFEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
WAFClient::~WAFClient()
{
}

std::shared_ptr<WAFEndpointProviderBase>& WAFClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void WAFClient::init(const WAF::WAFClientConfiguration& config)
{
  AWSClient::SetServiceClientName("WAF");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void WAFClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateByteMatchSetOutcome WAFClient::CreateByteMatchSet(const CreateByteMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateByteMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateByteMatchSetOutcomeCallable WAFClient::CreateByteMatchSetCallable(const CreateByteMatchSetRequest& request) const
{
  std::shared_ptr<CreateByteMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateByteMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateByteMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateByteMatchSetAsync(const CreateByteMatchSetRequest& request, const CreateByteMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateByteMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateByteMatchSet(*pRequest), context);
    } );
}

CreateGeoMatchSetOutcome WAFClient::CreateGeoMatchSet(const CreateGeoMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateGeoMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGeoMatchSetOutcomeCallable WAFClient::CreateGeoMatchSetCallable(const CreateGeoMatchSetRequest& request) const
{
  std::shared_ptr<CreateGeoMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateGeoMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateGeoMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateGeoMatchSetAsync(const CreateGeoMatchSetRequest& request, const CreateGeoMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateGeoMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateGeoMatchSet(*pRequest), context);
    } );
}

CreateIPSetOutcome WAFClient::CreateIPSet(const CreateIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateIPSetOutcomeCallable WAFClient::CreateIPSetCallable(const CreateIPSetRequest& request) const
{
  std::shared_ptr<CreateIPSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateIPSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateIPSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateIPSetAsync(const CreateIPSetRequest& request, const CreateIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateIPSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateIPSet(*pRequest), context);
    } );
}

CreateRateBasedRuleOutcome WAFClient::CreateRateBasedRule(const CreateRateBasedRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRateBasedRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRateBasedRuleOutcomeCallable WAFClient::CreateRateBasedRuleCallable(const CreateRateBasedRuleRequest& request) const
{
  std::shared_ptr<CreateRateBasedRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRateBasedRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRateBasedRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateRateBasedRuleAsync(const CreateRateBasedRuleRequest& request, const CreateRateBasedRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRateBasedRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRateBasedRule(*pRequest), context);
    } );
}

CreateRegexMatchSetOutcome WAFClient::CreateRegexMatchSet(const CreateRegexMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRegexMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRegexMatchSetOutcomeCallable WAFClient::CreateRegexMatchSetCallable(const CreateRegexMatchSetRequest& request) const
{
  std::shared_ptr<CreateRegexMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRegexMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRegexMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateRegexMatchSetAsync(const CreateRegexMatchSetRequest& request, const CreateRegexMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRegexMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRegexMatchSet(*pRequest), context);
    } );
}

CreateRegexPatternSetOutcome WAFClient::CreateRegexPatternSet(const CreateRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRegexPatternSetOutcomeCallable WAFClient::CreateRegexPatternSetCallable(const CreateRegexPatternSetRequest& request) const
{
  std::shared_ptr<CreateRegexPatternSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRegexPatternSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRegexPatternSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateRegexPatternSetAsync(const CreateRegexPatternSetRequest& request, const CreateRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRegexPatternSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRegexPatternSet(*pRequest), context);
    } );
}

CreateRuleOutcome WAFClient::CreateRule(const CreateRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRuleOutcomeCallable WAFClient::CreateRuleCallable(const CreateRuleRequest& request) const
{
  std::shared_ptr<CreateRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateRuleAsync(const CreateRuleRequest& request, const CreateRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRule(*pRequest), context);
    } );
}

CreateRuleGroupOutcome WAFClient::CreateRuleGroup(const CreateRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRuleGroupOutcomeCallable WAFClient::CreateRuleGroupCallable(const CreateRuleGroupRequest& request) const
{
  std::shared_ptr<CreateRuleGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRuleGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRuleGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateRuleGroupAsync(const CreateRuleGroupRequest& request, const CreateRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRuleGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRuleGroup(*pRequest), context);
    } );
}

CreateSizeConstraintSetOutcome WAFClient::CreateSizeConstraintSet(const CreateSizeConstraintSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSizeConstraintSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSizeConstraintSetOutcomeCallable WAFClient::CreateSizeConstraintSetCallable(const CreateSizeConstraintSetRequest& request) const
{
  std::shared_ptr<CreateSizeConstraintSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSizeConstraintSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSizeConstraintSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateSizeConstraintSetAsync(const CreateSizeConstraintSetRequest& request, const CreateSizeConstraintSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSizeConstraintSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSizeConstraintSet(*pRequest), context);
    } );
}

CreateSqlInjectionMatchSetOutcome WAFClient::CreateSqlInjectionMatchSet(const CreateSqlInjectionMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSqlInjectionMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSqlInjectionMatchSetOutcomeCallable WAFClient::CreateSqlInjectionMatchSetCallable(const CreateSqlInjectionMatchSetRequest& request) const
{
  std::shared_ptr<CreateSqlInjectionMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSqlInjectionMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSqlInjectionMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateSqlInjectionMatchSetAsync(const CreateSqlInjectionMatchSetRequest& request, const CreateSqlInjectionMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSqlInjectionMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSqlInjectionMatchSet(*pRequest), context);
    } );
}

CreateWebACLOutcome WAFClient::CreateWebACL(const CreateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWebACLOutcomeCallable WAFClient::CreateWebACLCallable(const CreateWebACLRequest& request) const
{
  std::shared_ptr<CreateWebACLRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateWebACLOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateWebACL(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateWebACLAsync(const CreateWebACLRequest& request, const CreateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateWebACLRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateWebACL(*pRequest), context);
    } );
}

CreateWebACLMigrationStackOutcome WAFClient::CreateWebACLMigrationStack(const CreateWebACLMigrationStackRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWebACLMigrationStack, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWebACLMigrationStack, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWebACLMigrationStackOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWebACLMigrationStackOutcomeCallable WAFClient::CreateWebACLMigrationStackCallable(const CreateWebACLMigrationStackRequest& request) const
{
  std::shared_ptr<CreateWebACLMigrationStackRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateWebACLMigrationStackOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateWebACLMigrationStack(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateWebACLMigrationStackAsync(const CreateWebACLMigrationStackRequest& request, const CreateWebACLMigrationStackResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateWebACLMigrationStackRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateWebACLMigrationStack(*pRequest), context);
    } );
}

CreateXssMatchSetOutcome WAFClient::CreateXssMatchSet(const CreateXssMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateXssMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateXssMatchSetOutcomeCallable WAFClient::CreateXssMatchSetCallable(const CreateXssMatchSetRequest& request) const
{
  std::shared_ptr<CreateXssMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateXssMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateXssMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::CreateXssMatchSetAsync(const CreateXssMatchSetRequest& request, const CreateXssMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateXssMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateXssMatchSet(*pRequest), context);
    } );
}

DeleteByteMatchSetOutcome WAFClient::DeleteByteMatchSet(const DeleteByteMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteByteMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteByteMatchSetOutcomeCallable WAFClient::DeleteByteMatchSetCallable(const DeleteByteMatchSetRequest& request) const
{
  std::shared_ptr<DeleteByteMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteByteMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteByteMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteByteMatchSetAsync(const DeleteByteMatchSetRequest& request, const DeleteByteMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteByteMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteByteMatchSet(*pRequest), context);
    } );
}

DeleteGeoMatchSetOutcome WAFClient::DeleteGeoMatchSet(const DeleteGeoMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteGeoMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteGeoMatchSetOutcomeCallable WAFClient::DeleteGeoMatchSetCallable(const DeleteGeoMatchSetRequest& request) const
{
  std::shared_ptr<DeleteGeoMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteGeoMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteGeoMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteGeoMatchSetAsync(const DeleteGeoMatchSetRequest& request, const DeleteGeoMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteGeoMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteGeoMatchSet(*pRequest), context);
    } );
}

DeleteIPSetOutcome WAFClient::DeleteIPSet(const DeleteIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteIPSetOutcomeCallable WAFClient::DeleteIPSetCallable(const DeleteIPSetRequest& request) const
{
  std::shared_ptr<DeleteIPSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteIPSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteIPSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteIPSetAsync(const DeleteIPSetRequest& request, const DeleteIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteIPSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteIPSet(*pRequest), context);
    } );
}

DeleteLoggingConfigurationOutcome WAFClient::DeleteLoggingConfiguration(const DeleteLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteLoggingConfigurationOutcomeCallable WAFClient::DeleteLoggingConfigurationCallable(const DeleteLoggingConfigurationRequest& request) const
{
  std::shared_ptr<DeleteLoggingConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteLoggingConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteLoggingConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteLoggingConfigurationAsync(const DeleteLoggingConfigurationRequest& request, const DeleteLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteLoggingConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteLoggingConfiguration(*pRequest), context);
    } );
}

DeletePermissionPolicyOutcome WAFClient::DeletePermissionPolicy(const DeletePermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePermissionPolicyOutcomeCallable WAFClient::DeletePermissionPolicyCallable(const DeletePermissionPolicyRequest& request) const
{
  std::shared_ptr<DeletePermissionPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeletePermissionPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeletePermissionPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeletePermissionPolicyAsync(const DeletePermissionPolicyRequest& request, const DeletePermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeletePermissionPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeletePermissionPolicy(*pRequest), context);
    } );
}

DeleteRateBasedRuleOutcome WAFClient::DeleteRateBasedRule(const DeleteRateBasedRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRateBasedRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRateBasedRuleOutcomeCallable WAFClient::DeleteRateBasedRuleCallable(const DeleteRateBasedRuleRequest& request) const
{
  std::shared_ptr<DeleteRateBasedRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRateBasedRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRateBasedRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteRateBasedRuleAsync(const DeleteRateBasedRuleRequest& request, const DeleteRateBasedRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRateBasedRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRateBasedRule(*pRequest), context);
    } );
}

DeleteRegexMatchSetOutcome WAFClient::DeleteRegexMatchSet(const DeleteRegexMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRegexMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRegexMatchSetOutcomeCallable WAFClient::DeleteRegexMatchSetCallable(const DeleteRegexMatchSetRequest& request) const
{
  std::shared_ptr<DeleteRegexMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRegexMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRegexMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteRegexMatchSetAsync(const DeleteRegexMatchSetRequest& request, const DeleteRegexMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRegexMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRegexMatchSet(*pRequest), context);
    } );
}

DeleteRegexPatternSetOutcome WAFClient::DeleteRegexPatternSet(const DeleteRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRegexPatternSetOutcomeCallable WAFClient::DeleteRegexPatternSetCallable(const DeleteRegexPatternSetRequest& request) const
{
  std::shared_ptr<DeleteRegexPatternSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRegexPatternSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRegexPatternSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteRegexPatternSetAsync(const DeleteRegexPatternSetRequest& request, const DeleteRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRegexPatternSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRegexPatternSet(*pRequest), context);
    } );
}

DeleteRuleOutcome WAFClient::DeleteRule(const DeleteRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRuleOutcomeCallable WAFClient::DeleteRuleCallable(const DeleteRuleRequest& request) const
{
  std::shared_ptr<DeleteRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteRuleAsync(const DeleteRuleRequest& request, const DeleteRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRule(*pRequest), context);
    } );
}

DeleteRuleGroupOutcome WAFClient::DeleteRuleGroup(const DeleteRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRuleGroupOutcomeCallable WAFClient::DeleteRuleGroupCallable(const DeleteRuleGroupRequest& request) const
{
  std::shared_ptr<DeleteRuleGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRuleGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRuleGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteRuleGroupAsync(const DeleteRuleGroupRequest& request, const DeleteRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRuleGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRuleGroup(*pRequest), context);
    } );
}

DeleteSizeConstraintSetOutcome WAFClient::DeleteSizeConstraintSet(const DeleteSizeConstraintSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSizeConstraintSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSizeConstraintSetOutcomeCallable WAFClient::DeleteSizeConstraintSetCallable(const DeleteSizeConstraintSetRequest& request) const
{
  std::shared_ptr<DeleteSizeConstraintSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSizeConstraintSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSizeConstraintSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteSizeConstraintSetAsync(const DeleteSizeConstraintSetRequest& request, const DeleteSizeConstraintSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSizeConstraintSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSizeConstraintSet(*pRequest), context);
    } );
}

DeleteSqlInjectionMatchSetOutcome WAFClient::DeleteSqlInjectionMatchSet(const DeleteSqlInjectionMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSqlInjectionMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSqlInjectionMatchSetOutcomeCallable WAFClient::DeleteSqlInjectionMatchSetCallable(const DeleteSqlInjectionMatchSetRequest& request) const
{
  std::shared_ptr<DeleteSqlInjectionMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSqlInjectionMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSqlInjectionMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteSqlInjectionMatchSetAsync(const DeleteSqlInjectionMatchSetRequest& request, const DeleteSqlInjectionMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSqlInjectionMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSqlInjectionMatchSet(*pRequest), context);
    } );
}

DeleteWebACLOutcome WAFClient::DeleteWebACL(const DeleteWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteWebACLOutcomeCallable WAFClient::DeleteWebACLCallable(const DeleteWebACLRequest& request) const
{
  std::shared_ptr<DeleteWebACLRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteWebACLOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteWebACL(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteWebACLAsync(const DeleteWebACLRequest& request, const DeleteWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteWebACLRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteWebACL(*pRequest), context);
    } );
}

DeleteXssMatchSetOutcome WAFClient::DeleteXssMatchSet(const DeleteXssMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteXssMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteXssMatchSetOutcomeCallable WAFClient::DeleteXssMatchSetCallable(const DeleteXssMatchSetRequest& request) const
{
  std::shared_ptr<DeleteXssMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteXssMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteXssMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::DeleteXssMatchSetAsync(const DeleteXssMatchSetRequest& request, const DeleteXssMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteXssMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteXssMatchSet(*pRequest), context);
    } );
}

GetByteMatchSetOutcome WAFClient::GetByteMatchSet(const GetByteMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetByteMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetByteMatchSetOutcomeCallable WAFClient::GetByteMatchSetCallable(const GetByteMatchSetRequest& request) const
{
  std::shared_ptr<GetByteMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetByteMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetByteMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetByteMatchSetAsync(const GetByteMatchSetRequest& request, const GetByteMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetByteMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetByteMatchSet(*pRequest), context);
    } );
}

GetChangeTokenOutcome WAFClient::GetChangeToken(const GetChangeTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChangeToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChangeToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetChangeTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetChangeTokenOutcomeCallable WAFClient::GetChangeTokenCallable(const GetChangeTokenRequest& request) const
{
  std::shared_ptr<GetChangeTokenRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetChangeTokenOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetChangeToken(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetChangeTokenAsync(const GetChangeTokenRequest& request, const GetChangeTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetChangeTokenRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetChangeToken(*pRequest), context);
    } );
}

GetChangeTokenStatusOutcome WAFClient::GetChangeTokenStatus(const GetChangeTokenStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetChangeTokenStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetChangeTokenStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetChangeTokenStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetChangeTokenStatusOutcomeCallable WAFClient::GetChangeTokenStatusCallable(const GetChangeTokenStatusRequest& request) const
{
  std::shared_ptr<GetChangeTokenStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetChangeTokenStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetChangeTokenStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetChangeTokenStatusAsync(const GetChangeTokenStatusRequest& request, const GetChangeTokenStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetChangeTokenStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetChangeTokenStatus(*pRequest), context);
    } );
}

GetGeoMatchSetOutcome WAFClient::GetGeoMatchSet(const GetGeoMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetGeoMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGeoMatchSetOutcomeCallable WAFClient::GetGeoMatchSetCallable(const GetGeoMatchSetRequest& request) const
{
  std::shared_ptr<GetGeoMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetGeoMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetGeoMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetGeoMatchSetAsync(const GetGeoMatchSetRequest& request, const GetGeoMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetGeoMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetGeoMatchSet(*pRequest), context);
    } );
}

GetIPSetOutcome WAFClient::GetIPSet(const GetIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetIPSetOutcomeCallable WAFClient::GetIPSetCallable(const GetIPSetRequest& request) const
{
  std::shared_ptr<GetIPSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetIPSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetIPSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetIPSetAsync(const GetIPSetRequest& request, const GetIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetIPSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetIPSet(*pRequest), context);
    } );
}

GetLoggingConfigurationOutcome WAFClient::GetLoggingConfiguration(const GetLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetLoggingConfigurationOutcomeCallable WAFClient::GetLoggingConfigurationCallable(const GetLoggingConfigurationRequest& request) const
{
  std::shared_ptr<GetLoggingConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetLoggingConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetLoggingConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetLoggingConfigurationAsync(const GetLoggingConfigurationRequest& request, const GetLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetLoggingConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetLoggingConfiguration(*pRequest), context);
    } );
}

GetPermissionPolicyOutcome WAFClient::GetPermissionPolicy(const GetPermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetPermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetPermissionPolicyOutcomeCallable WAFClient::GetPermissionPolicyCallable(const GetPermissionPolicyRequest& request) const
{
  std::shared_ptr<GetPermissionPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetPermissionPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetPermissionPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetPermissionPolicyAsync(const GetPermissionPolicyRequest& request, const GetPermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetPermissionPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetPermissionPolicy(*pRequest), context);
    } );
}

GetRateBasedRuleOutcome WAFClient::GetRateBasedRule(const GetRateBasedRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRateBasedRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRateBasedRuleOutcomeCallable WAFClient::GetRateBasedRuleCallable(const GetRateBasedRuleRequest& request) const
{
  std::shared_ptr<GetRateBasedRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRateBasedRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRateBasedRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRateBasedRuleAsync(const GetRateBasedRuleRequest& request, const GetRateBasedRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRateBasedRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRateBasedRule(*pRequest), context);
    } );
}

GetRateBasedRuleManagedKeysOutcome WAFClient::GetRateBasedRuleManagedKeys(const GetRateBasedRuleManagedKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRateBasedRuleManagedKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRateBasedRuleManagedKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRateBasedRuleManagedKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRateBasedRuleManagedKeysOutcomeCallable WAFClient::GetRateBasedRuleManagedKeysCallable(const GetRateBasedRuleManagedKeysRequest& request) const
{
  std::shared_ptr<GetRateBasedRuleManagedKeysRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRateBasedRuleManagedKeysOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRateBasedRuleManagedKeys(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRateBasedRuleManagedKeysAsync(const GetRateBasedRuleManagedKeysRequest& request, const GetRateBasedRuleManagedKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRateBasedRuleManagedKeysRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRateBasedRuleManagedKeys(*pRequest), context);
    } );
}

GetRegexMatchSetOutcome WAFClient::GetRegexMatchSet(const GetRegexMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRegexMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRegexMatchSetOutcomeCallable WAFClient::GetRegexMatchSetCallable(const GetRegexMatchSetRequest& request) const
{
  std::shared_ptr<GetRegexMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRegexMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRegexMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRegexMatchSetAsync(const GetRegexMatchSetRequest& request, const GetRegexMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRegexMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRegexMatchSet(*pRequest), context);
    } );
}

GetRegexPatternSetOutcome WAFClient::GetRegexPatternSet(const GetRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRegexPatternSetOutcomeCallable WAFClient::GetRegexPatternSetCallable(const GetRegexPatternSetRequest& request) const
{
  std::shared_ptr<GetRegexPatternSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRegexPatternSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRegexPatternSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRegexPatternSetAsync(const GetRegexPatternSetRequest& request, const GetRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRegexPatternSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRegexPatternSet(*pRequest), context);
    } );
}

GetRuleOutcome WAFClient::GetRule(const GetRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRuleOutcomeCallable WAFClient::GetRuleCallable(const GetRuleRequest& request) const
{
  std::shared_ptr<GetRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRuleAsync(const GetRuleRequest& request, const GetRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRule(*pRequest), context);
    } );
}

GetRuleGroupOutcome WAFClient::GetRuleGroup(const GetRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetRuleGroupOutcomeCallable WAFClient::GetRuleGroupCallable(const GetRuleGroupRequest& request) const
{
  std::shared_ptr<GetRuleGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetRuleGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetRuleGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetRuleGroupAsync(const GetRuleGroupRequest& request, const GetRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetRuleGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetRuleGroup(*pRequest), context);
    } );
}

GetSampledRequestsOutcome WAFClient::GetSampledRequests(const GetSampledRequestsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSampledRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSampledRequests, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSampledRequestsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSampledRequestsOutcomeCallable WAFClient::GetSampledRequestsCallable(const GetSampledRequestsRequest& request) const
{
  std::shared_ptr<GetSampledRequestsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetSampledRequestsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetSampledRequests(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetSampledRequestsAsync(const GetSampledRequestsRequest& request, const GetSampledRequestsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetSampledRequestsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetSampledRequests(*pRequest), context);
    } );
}

GetSizeConstraintSetOutcome WAFClient::GetSizeConstraintSet(const GetSizeConstraintSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSizeConstraintSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSizeConstraintSetOutcomeCallable WAFClient::GetSizeConstraintSetCallable(const GetSizeConstraintSetRequest& request) const
{
  std::shared_ptr<GetSizeConstraintSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetSizeConstraintSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetSizeConstraintSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetSizeConstraintSetAsync(const GetSizeConstraintSetRequest& request, const GetSizeConstraintSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetSizeConstraintSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetSizeConstraintSet(*pRequest), context);
    } );
}

GetSqlInjectionMatchSetOutcome WAFClient::GetSqlInjectionMatchSet(const GetSqlInjectionMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSqlInjectionMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSqlInjectionMatchSetOutcomeCallable WAFClient::GetSqlInjectionMatchSetCallable(const GetSqlInjectionMatchSetRequest& request) const
{
  std::shared_ptr<GetSqlInjectionMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetSqlInjectionMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetSqlInjectionMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetSqlInjectionMatchSetAsync(const GetSqlInjectionMatchSetRequest& request, const GetSqlInjectionMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetSqlInjectionMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetSqlInjectionMatchSet(*pRequest), context);
    } );
}

GetWebACLOutcome WAFClient::GetWebACL(const GetWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetWebACLOutcomeCallable WAFClient::GetWebACLCallable(const GetWebACLRequest& request) const
{
  std::shared_ptr<GetWebACLRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetWebACLOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetWebACL(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetWebACLAsync(const GetWebACLRequest& request, const GetWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetWebACLRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetWebACL(*pRequest), context);
    } );
}

GetXssMatchSetOutcome WAFClient::GetXssMatchSet(const GetXssMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetXssMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetXssMatchSetOutcomeCallable WAFClient::GetXssMatchSetCallable(const GetXssMatchSetRequest& request) const
{
  std::shared_ptr<GetXssMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetXssMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetXssMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::GetXssMatchSetAsync(const GetXssMatchSetRequest& request, const GetXssMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetXssMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetXssMatchSet(*pRequest), context);
    } );
}

ListActivatedRulesInRuleGroupOutcome WAFClient::ListActivatedRulesInRuleGroup(const ListActivatedRulesInRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListActivatedRulesInRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListActivatedRulesInRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListActivatedRulesInRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListActivatedRulesInRuleGroupOutcomeCallable WAFClient::ListActivatedRulesInRuleGroupCallable(const ListActivatedRulesInRuleGroupRequest& request) const
{
  std::shared_ptr<ListActivatedRulesInRuleGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListActivatedRulesInRuleGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListActivatedRulesInRuleGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListActivatedRulesInRuleGroupAsync(const ListActivatedRulesInRuleGroupRequest& request, const ListActivatedRulesInRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListActivatedRulesInRuleGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListActivatedRulesInRuleGroup(*pRequest), context);
    } );
}

ListByteMatchSetsOutcome WAFClient::ListByteMatchSets(const ListByteMatchSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListByteMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListByteMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListByteMatchSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListByteMatchSetsOutcomeCallable WAFClient::ListByteMatchSetsCallable(const ListByteMatchSetsRequest& request) const
{
  std::shared_ptr<ListByteMatchSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListByteMatchSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListByteMatchSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListByteMatchSetsAsync(const ListByteMatchSetsRequest& request, const ListByteMatchSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListByteMatchSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListByteMatchSets(*pRequest), context);
    } );
}

ListGeoMatchSetsOutcome WAFClient::ListGeoMatchSets(const ListGeoMatchSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGeoMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGeoMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListGeoMatchSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGeoMatchSetsOutcomeCallable WAFClient::ListGeoMatchSetsCallable(const ListGeoMatchSetsRequest& request) const
{
  std::shared_ptr<ListGeoMatchSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListGeoMatchSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListGeoMatchSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListGeoMatchSetsAsync(const ListGeoMatchSetsRequest& request, const ListGeoMatchSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListGeoMatchSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListGeoMatchSets(*pRequest), context);
    } );
}

ListIPSetsOutcome WAFClient::ListIPSets(const ListIPSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIPSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIPSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListIPSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListIPSetsOutcomeCallable WAFClient::ListIPSetsCallable(const ListIPSetsRequest& request) const
{
  std::shared_ptr<ListIPSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListIPSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListIPSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListIPSetsAsync(const ListIPSetsRequest& request, const ListIPSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListIPSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListIPSets(*pRequest), context);
    } );
}

ListLoggingConfigurationsOutcome WAFClient::ListLoggingConfigurations(const ListLoggingConfigurationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListLoggingConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListLoggingConfigurationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListLoggingConfigurationsOutcomeCallable WAFClient::ListLoggingConfigurationsCallable(const ListLoggingConfigurationsRequest& request) const
{
  std::shared_ptr<ListLoggingConfigurationsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListLoggingConfigurationsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListLoggingConfigurations(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListLoggingConfigurationsAsync(const ListLoggingConfigurationsRequest& request, const ListLoggingConfigurationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListLoggingConfigurationsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListLoggingConfigurations(*pRequest), context);
    } );
}

ListRateBasedRulesOutcome WAFClient::ListRateBasedRules(const ListRateBasedRulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRateBasedRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRateBasedRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRateBasedRulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRateBasedRulesOutcomeCallable WAFClient::ListRateBasedRulesCallable(const ListRateBasedRulesRequest& request) const
{
  std::shared_ptr<ListRateBasedRulesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRateBasedRulesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRateBasedRules(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListRateBasedRulesAsync(const ListRateBasedRulesRequest& request, const ListRateBasedRulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRateBasedRulesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRateBasedRules(*pRequest), context);
    } );
}

ListRegexMatchSetsOutcome WAFClient::ListRegexMatchSets(const ListRegexMatchSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRegexMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRegexMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRegexMatchSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRegexMatchSetsOutcomeCallable WAFClient::ListRegexMatchSetsCallable(const ListRegexMatchSetsRequest& request) const
{
  std::shared_ptr<ListRegexMatchSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRegexMatchSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRegexMatchSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListRegexMatchSetsAsync(const ListRegexMatchSetsRequest& request, const ListRegexMatchSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRegexMatchSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRegexMatchSets(*pRequest), context);
    } );
}

ListRegexPatternSetsOutcome WAFClient::ListRegexPatternSets(const ListRegexPatternSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRegexPatternSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRegexPatternSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRegexPatternSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRegexPatternSetsOutcomeCallable WAFClient::ListRegexPatternSetsCallable(const ListRegexPatternSetsRequest& request) const
{
  std::shared_ptr<ListRegexPatternSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRegexPatternSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRegexPatternSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListRegexPatternSetsAsync(const ListRegexPatternSetsRequest& request, const ListRegexPatternSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRegexPatternSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRegexPatternSets(*pRequest), context);
    } );
}

ListRuleGroupsOutcome WAFClient::ListRuleGroups(const ListRuleGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRuleGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRuleGroupsOutcomeCallable WAFClient::ListRuleGroupsCallable(const ListRuleGroupsRequest& request) const
{
  std::shared_ptr<ListRuleGroupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRuleGroupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRuleGroups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListRuleGroupsAsync(const ListRuleGroupsRequest& request, const ListRuleGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRuleGroupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRuleGroups(*pRequest), context);
    } );
}

ListRulesOutcome WAFClient::ListRules(const ListRulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListRulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRulesOutcomeCallable WAFClient::ListRulesCallable(const ListRulesRequest& request) const
{
  std::shared_ptr<ListRulesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRulesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRules(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListRulesAsync(const ListRulesRequest& request, const ListRulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRulesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRules(*pRequest), context);
    } );
}

ListSizeConstraintSetsOutcome WAFClient::ListSizeConstraintSets(const ListSizeConstraintSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSizeConstraintSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSizeConstraintSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListSizeConstraintSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSizeConstraintSetsOutcomeCallable WAFClient::ListSizeConstraintSetsCallable(const ListSizeConstraintSetsRequest& request) const
{
  std::shared_ptr<ListSizeConstraintSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSizeConstraintSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSizeConstraintSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListSizeConstraintSetsAsync(const ListSizeConstraintSetsRequest& request, const ListSizeConstraintSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSizeConstraintSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSizeConstraintSets(*pRequest), context);
    } );
}

ListSqlInjectionMatchSetsOutcome WAFClient::ListSqlInjectionMatchSets(const ListSqlInjectionMatchSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSqlInjectionMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSqlInjectionMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListSqlInjectionMatchSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSqlInjectionMatchSetsOutcomeCallable WAFClient::ListSqlInjectionMatchSetsCallable(const ListSqlInjectionMatchSetsRequest& request) const
{
  std::shared_ptr<ListSqlInjectionMatchSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSqlInjectionMatchSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSqlInjectionMatchSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListSqlInjectionMatchSetsAsync(const ListSqlInjectionMatchSetsRequest& request, const ListSqlInjectionMatchSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSqlInjectionMatchSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSqlInjectionMatchSets(*pRequest), context);
    } );
}

ListSubscribedRuleGroupsOutcome WAFClient::ListSubscribedRuleGroups(const ListSubscribedRuleGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSubscribedRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSubscribedRuleGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListSubscribedRuleGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSubscribedRuleGroupsOutcomeCallable WAFClient::ListSubscribedRuleGroupsCallable(const ListSubscribedRuleGroupsRequest& request) const
{
  std::shared_ptr<ListSubscribedRuleGroupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSubscribedRuleGroupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSubscribedRuleGroups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListSubscribedRuleGroupsAsync(const ListSubscribedRuleGroupsRequest& request, const ListSubscribedRuleGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSubscribedRuleGroupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSubscribedRuleGroups(*pRequest), context);
    } );
}

ListTagsForResourceOutcome WAFClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable WAFClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListTagsForResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListTagsForResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListTagsForResource(*pRequest), context);
    } );
}

ListWebACLsOutcome WAFClient::ListWebACLs(const ListWebACLsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWebACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWebACLs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListWebACLsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWebACLsOutcomeCallable WAFClient::ListWebACLsCallable(const ListWebACLsRequest& request) const
{
  std::shared_ptr<ListWebACLsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListWebACLsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListWebACLs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListWebACLsAsync(const ListWebACLsRequest& request, const ListWebACLsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListWebACLsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListWebACLs(*pRequest), context);
    } );
}

ListXssMatchSetsOutcome WAFClient::ListXssMatchSets(const ListXssMatchSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListXssMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListXssMatchSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListXssMatchSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListXssMatchSetsOutcomeCallable WAFClient::ListXssMatchSetsCallable(const ListXssMatchSetsRequest& request) const
{
  std::shared_ptr<ListXssMatchSetsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListXssMatchSetsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListXssMatchSets(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::ListXssMatchSetsAsync(const ListXssMatchSetsRequest& request, const ListXssMatchSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListXssMatchSetsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListXssMatchSets(*pRequest), context);
    } );
}

PutLoggingConfigurationOutcome WAFClient::PutLoggingConfiguration(const PutLoggingConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutLoggingConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutLoggingConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutLoggingConfigurationOutcomeCallable WAFClient::PutLoggingConfigurationCallable(const PutLoggingConfigurationRequest& request) const
{
  std::shared_ptr<PutLoggingConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< PutLoggingConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->PutLoggingConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::PutLoggingConfigurationAsync(const PutLoggingConfigurationRequest& request, const PutLoggingConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<PutLoggingConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, PutLoggingConfiguration(*pRequest), context);
    } );
}

PutPermissionPolicyOutcome WAFClient::PutPermissionPolicy(const PutPermissionPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutPermissionPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutPermissionPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutPermissionPolicyOutcomeCallable WAFClient::PutPermissionPolicyCallable(const PutPermissionPolicyRequest& request) const
{
  std::shared_ptr<PutPermissionPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< PutPermissionPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->PutPermissionPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::PutPermissionPolicyAsync(const PutPermissionPolicyRequest& request, const PutPermissionPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<PutPermissionPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, PutPermissionPolicy(*pRequest), context);
    } );
}

TagResourceOutcome WAFClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable WAFClient::TagResourceCallable(const TagResourceRequest& request) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TagResource(*pRequest), context);
    } );
}

UntagResourceOutcome WAFClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable WAFClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UntagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UntagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UntagResource(*pRequest), context);
    } );
}

UpdateByteMatchSetOutcome WAFClient::UpdateByteMatchSet(const UpdateByteMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateByteMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateByteMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateByteMatchSetOutcomeCallable WAFClient::UpdateByteMatchSetCallable(const UpdateByteMatchSetRequest& request) const
{
  std::shared_ptr<UpdateByteMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateByteMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateByteMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateByteMatchSetAsync(const UpdateByteMatchSetRequest& request, const UpdateByteMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateByteMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateByteMatchSet(*pRequest), context);
    } );
}

UpdateGeoMatchSetOutcome WAFClient::UpdateGeoMatchSet(const UpdateGeoMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGeoMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateGeoMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGeoMatchSetOutcomeCallable WAFClient::UpdateGeoMatchSetCallable(const UpdateGeoMatchSetRequest& request) const
{
  std::shared_ptr<UpdateGeoMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateGeoMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateGeoMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateGeoMatchSetAsync(const UpdateGeoMatchSetRequest& request, const UpdateGeoMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateGeoMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateGeoMatchSet(*pRequest), context);
    } );
}

UpdateIPSetOutcome WAFClient::UpdateIPSet(const UpdateIPSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateIPSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateIPSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateIPSetOutcomeCallable WAFClient::UpdateIPSetCallable(const UpdateIPSetRequest& request) const
{
  std::shared_ptr<UpdateIPSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateIPSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateIPSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateIPSetAsync(const UpdateIPSetRequest& request, const UpdateIPSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateIPSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateIPSet(*pRequest), context);
    } );
}

UpdateRateBasedRuleOutcome WAFClient::UpdateRateBasedRule(const UpdateRateBasedRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRateBasedRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRateBasedRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRateBasedRuleOutcomeCallable WAFClient::UpdateRateBasedRuleCallable(const UpdateRateBasedRuleRequest& request) const
{
  std::shared_ptr<UpdateRateBasedRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRateBasedRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRateBasedRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateRateBasedRuleAsync(const UpdateRateBasedRuleRequest& request, const UpdateRateBasedRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRateBasedRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRateBasedRule(*pRequest), context);
    } );
}

UpdateRegexMatchSetOutcome WAFClient::UpdateRegexMatchSet(const UpdateRegexMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRegexMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRegexMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRegexMatchSetOutcomeCallable WAFClient::UpdateRegexMatchSetCallable(const UpdateRegexMatchSetRequest& request) const
{
  std::shared_ptr<UpdateRegexMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRegexMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRegexMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateRegexMatchSetAsync(const UpdateRegexMatchSetRequest& request, const UpdateRegexMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRegexMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRegexMatchSet(*pRequest), context);
    } );
}

UpdateRegexPatternSetOutcome WAFClient::UpdateRegexPatternSet(const UpdateRegexPatternSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRegexPatternSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRegexPatternSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRegexPatternSetOutcomeCallable WAFClient::UpdateRegexPatternSetCallable(const UpdateRegexPatternSetRequest& request) const
{
  std::shared_ptr<UpdateRegexPatternSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRegexPatternSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRegexPatternSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateRegexPatternSetAsync(const UpdateRegexPatternSetRequest& request, const UpdateRegexPatternSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRegexPatternSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRegexPatternSet(*pRequest), context);
    } );
}

UpdateRuleOutcome WAFClient::UpdateRule(const UpdateRuleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRuleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRuleOutcomeCallable WAFClient::UpdateRuleCallable(const UpdateRuleRequest& request) const
{
  std::shared_ptr<UpdateRuleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRuleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateRuleAsync(const UpdateRuleRequest& request, const UpdateRuleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRuleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRule(*pRequest), context);
    } );
}

UpdateRuleGroupOutcome WAFClient::UpdateRuleGroup(const UpdateRuleGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRuleGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRuleGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRuleGroupOutcomeCallable WAFClient::UpdateRuleGroupCallable(const UpdateRuleGroupRequest& request) const
{
  std::shared_ptr<UpdateRuleGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRuleGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRuleGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateRuleGroupAsync(const UpdateRuleGroupRequest& request, const UpdateRuleGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRuleGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRuleGroup(*pRequest), context);
    } );
}

UpdateSizeConstraintSetOutcome WAFClient::UpdateSizeConstraintSet(const UpdateSizeConstraintSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSizeConstraintSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateSizeConstraintSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateSizeConstraintSetOutcomeCallable WAFClient::UpdateSizeConstraintSetCallable(const UpdateSizeConstraintSetRequest& request) const
{
  std::shared_ptr<UpdateSizeConstraintSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateSizeConstraintSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateSizeConstraintSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateSizeConstraintSetAsync(const UpdateSizeConstraintSetRequest& request, const UpdateSizeConstraintSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateSizeConstraintSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateSizeConstraintSet(*pRequest), context);
    } );
}

UpdateSqlInjectionMatchSetOutcome WAFClient::UpdateSqlInjectionMatchSet(const UpdateSqlInjectionMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSqlInjectionMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateSqlInjectionMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateSqlInjectionMatchSetOutcomeCallable WAFClient::UpdateSqlInjectionMatchSetCallable(const UpdateSqlInjectionMatchSetRequest& request) const
{
  std::shared_ptr<UpdateSqlInjectionMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateSqlInjectionMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateSqlInjectionMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateSqlInjectionMatchSetAsync(const UpdateSqlInjectionMatchSetRequest& request, const UpdateSqlInjectionMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateSqlInjectionMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateSqlInjectionMatchSet(*pRequest), context);
    } );
}

UpdateWebACLOutcome WAFClient::UpdateWebACL(const UpdateWebACLRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateWebACL, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateWebACLOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateWebACLOutcomeCallable WAFClient::UpdateWebACLCallable(const UpdateWebACLRequest& request) const
{
  std::shared_ptr<UpdateWebACLRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateWebACLOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateWebACL(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateWebACLAsync(const UpdateWebACLRequest& request, const UpdateWebACLResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateWebACLRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateWebACL(*pRequest), context);
    } );
}

UpdateXssMatchSetOutcome WAFClient::UpdateXssMatchSet(const UpdateXssMatchSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateXssMatchSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateXssMatchSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateXssMatchSetOutcomeCallable WAFClient::UpdateXssMatchSetCallable(const UpdateXssMatchSetRequest& request) const
{
  std::shared_ptr<UpdateXssMatchSetRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateXssMatchSetOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateXssMatchSet(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void WAFClient::UpdateXssMatchSetAsync(const UpdateXssMatchSetRequest& request, const UpdateXssMatchSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateXssMatchSetRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateXssMatchSet(*pRequest), context);
    } );
}

