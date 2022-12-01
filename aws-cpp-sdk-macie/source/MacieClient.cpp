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

#include <aws/macie/MacieClient.h>
#include <aws/macie/MacieErrorMarshaller.h>
#include <aws/macie/MacieEndpointProvider.h>
#include <aws/macie/model/AssociateMemberAccountRequest.h>
#include <aws/macie/model/AssociateS3ResourcesRequest.h>
#include <aws/macie/model/DisassociateMemberAccountRequest.h>
#include <aws/macie/model/DisassociateS3ResourcesRequest.h>
#include <aws/macie/model/ListMemberAccountsRequest.h>
#include <aws/macie/model/ListS3ResourcesRequest.h>
#include <aws/macie/model/UpdateS3ResourcesRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Macie;
using namespace Aws::Macie::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* MacieClient::SERVICE_NAME = "macie";
const char* MacieClient::ALLOCATION_TAG = "MacieClient";

MacieClient::MacieClient(const Macie::MacieClientConfiguration& clientConfiguration,
                         std::shared_ptr<MacieEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MacieClient::MacieClient(const AWSCredentials& credentials,
                         std::shared_ptr<MacieEndpointProviderBase> endpointProvider,
                         const Macie::MacieClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MacieClient::MacieClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         std::shared_ptr<MacieEndpointProviderBase> endpointProvider,
                         const Macie::MacieClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  MacieClient::MacieClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<MacieEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MacieClient::MacieClient(const AWSCredentials& credentials,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MacieEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MacieClient::MacieClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MacieErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MacieEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
MacieClient::~MacieClient()
{
}

std::shared_ptr<MacieEndpointProviderBase>& MacieClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void MacieClient::init(const Macie::MacieClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Macie");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void MacieClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateMemberAccountOutcome MacieClient::AssociateMemberAccount(const AssociateMemberAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateMemberAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateMemberAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateMemberAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateMemberAccountOutcomeCallable MacieClient::AssociateMemberAccountCallable(const AssociateMemberAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::AssociateMemberAccount, this, request, m_executor.get());
}

void MacieClient::AssociateMemberAccountAsync(const AssociateMemberAccountRequest& request, const AssociateMemberAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::AssociateMemberAccount, this, request, handler, context, m_executor.get());
}

AssociateS3ResourcesOutcome MacieClient::AssociateS3Resources(const AssociateS3ResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateS3ResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateS3ResourcesOutcomeCallable MacieClient::AssociateS3ResourcesCallable(const AssociateS3ResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::AssociateS3Resources, this, request, m_executor.get());
}

void MacieClient::AssociateS3ResourcesAsync(const AssociateS3ResourcesRequest& request, const AssociateS3ResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::AssociateS3Resources, this, request, handler, context, m_executor.get());
}

DisassociateMemberAccountOutcome MacieClient::DisassociateMemberAccount(const DisassociateMemberAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateMemberAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateMemberAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateMemberAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateMemberAccountOutcomeCallable MacieClient::DisassociateMemberAccountCallable(const DisassociateMemberAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::DisassociateMemberAccount, this, request, m_executor.get());
}

void MacieClient::DisassociateMemberAccountAsync(const DisassociateMemberAccountRequest& request, const DisassociateMemberAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::DisassociateMemberAccount, this, request, handler, context, m_executor.get());
}

DisassociateS3ResourcesOutcome MacieClient::DisassociateS3Resources(const DisassociateS3ResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateS3ResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateS3ResourcesOutcomeCallable MacieClient::DisassociateS3ResourcesCallable(const DisassociateS3ResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::DisassociateS3Resources, this, request, m_executor.get());
}

void MacieClient::DisassociateS3ResourcesAsync(const DisassociateS3ResourcesRequest& request, const DisassociateS3ResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::DisassociateS3Resources, this, request, handler, context, m_executor.get());
}

ListMemberAccountsOutcome MacieClient::ListMemberAccounts(const ListMemberAccountsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMemberAccounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMemberAccounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListMemberAccountsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListMemberAccountsOutcomeCallable MacieClient::ListMemberAccountsCallable(const ListMemberAccountsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::ListMemberAccounts, this, request, m_executor.get());
}

void MacieClient::ListMemberAccountsAsync(const ListMemberAccountsRequest& request, const ListMemberAccountsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::ListMemberAccounts, this, request, handler, context, m_executor.get());
}

ListS3ResourcesOutcome MacieClient::ListS3Resources(const ListS3ResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListS3ResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListS3ResourcesOutcomeCallable MacieClient::ListS3ResourcesCallable(const ListS3ResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::ListS3Resources, this, request, m_executor.get());
}

void MacieClient::ListS3ResourcesAsync(const ListS3ResourcesRequest& request, const ListS3ResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::ListS3Resources, this, request, handler, context, m_executor.get());
}

UpdateS3ResourcesOutcome MacieClient::UpdateS3Resources(const UpdateS3ResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateS3Resources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateS3ResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateS3ResourcesOutcomeCallable MacieClient::UpdateS3ResourcesCallable(const UpdateS3ResourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &MacieClient::UpdateS3Resources, this, request, m_executor.get());
}

void MacieClient::UpdateS3ResourcesAsync(const UpdateS3ResourcesRequest& request, const UpdateS3ResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&MacieClient::UpdateS3Resources, this, request, handler, context, m_executor.get());
}

