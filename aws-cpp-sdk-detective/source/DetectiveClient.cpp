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

#include <aws/detective/DetectiveClient.h>
#include <aws/detective/DetectiveErrorMarshaller.h>
#include <aws/detective/DetectiveEndpointProvider.h>
#include <aws/detective/model/AcceptInvitationRequest.h>
#include <aws/detective/model/BatchGetGraphMemberDatasourcesRequest.h>
#include <aws/detective/model/BatchGetMembershipDatasourcesRequest.h>
#include <aws/detective/model/CreateGraphRequest.h>
#include <aws/detective/model/CreateMembersRequest.h>
#include <aws/detective/model/DeleteGraphRequest.h>
#include <aws/detective/model/DeleteMembersRequest.h>
#include <aws/detective/model/DescribeOrganizationConfigurationRequest.h>
#include <aws/detective/model/DisassociateMembershipRequest.h>
#include <aws/detective/model/EnableOrganizationAdminAccountRequest.h>
#include <aws/detective/model/GetMembersRequest.h>
#include <aws/detective/model/ListDatasourcePackagesRequest.h>
#include <aws/detective/model/ListGraphsRequest.h>
#include <aws/detective/model/ListInvitationsRequest.h>
#include <aws/detective/model/ListMembersRequest.h>
#include <aws/detective/model/ListOrganizationAdminAccountsRequest.h>
#include <aws/detective/model/ListTagsForResourceRequest.h>
#include <aws/detective/model/RejectInvitationRequest.h>
#include <aws/detective/model/StartMonitoringMemberRequest.h>
#include <aws/detective/model/TagResourceRequest.h>
#include <aws/detective/model/UntagResourceRequest.h>
#include <aws/detective/model/UpdateDatasourcePackagesRequest.h>
#include <aws/detective/model/UpdateOrganizationConfigurationRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Detective;
using namespace Aws::Detective::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* DetectiveClient::SERVICE_NAME = "detective";
const char* DetectiveClient::ALLOCATION_TAG = "DetectiveClient";

DetectiveClient::DetectiveClient(const Detective::DetectiveClientConfiguration& clientConfiguration,
                                 std::shared_ptr<DetectiveEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

DetectiveClient::DetectiveClient(const AWSCredentials& credentials,
                                 std::shared_ptr<DetectiveEndpointProviderBase> endpointProvider,
                                 const Detective::DetectiveClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

DetectiveClient::DetectiveClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 std::shared_ptr<DetectiveEndpointProviderBase> endpointProvider,
                                 const Detective::DetectiveClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  DetectiveClient::DetectiveClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<DetectiveEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

DetectiveClient::DetectiveClient(const AWSCredentials& credentials,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<DetectiveEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

DetectiveClient::DetectiveClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<DetectiveErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<DetectiveEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
DetectiveClient::~DetectiveClient()
{
}

std::shared_ptr<DetectiveEndpointProviderBase>& DetectiveClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void DetectiveClient::init(const Detective::DetectiveClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Detective");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void DetectiveClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AcceptInvitationOutcome DetectiveClient::AcceptInvitation(const AcceptInvitationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AcceptInvitation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AcceptInvitation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/invitation");
  return AcceptInvitationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

AcceptInvitationOutcomeCallable DetectiveClient::AcceptInvitationCallable(const AcceptInvitationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::AcceptInvitation, this, request, m_executor.get());
}

void DetectiveClient::AcceptInvitationAsync(const AcceptInvitationRequest& request, const AcceptInvitationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::AcceptInvitation, this, request, handler, context, m_executor.get());
}

BatchGetGraphMemberDatasourcesOutcome DetectiveClient::BatchGetGraphMemberDatasources(const BatchGetGraphMemberDatasourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetGraphMemberDatasources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetGraphMemberDatasources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/datasources/get");
  return BatchGetGraphMemberDatasourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetGraphMemberDatasourcesOutcomeCallable DetectiveClient::BatchGetGraphMemberDatasourcesCallable(const BatchGetGraphMemberDatasourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::BatchGetGraphMemberDatasources, this, request, m_executor.get());
}

void DetectiveClient::BatchGetGraphMemberDatasourcesAsync(const BatchGetGraphMemberDatasourcesRequest& request, const BatchGetGraphMemberDatasourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::BatchGetGraphMemberDatasources, this, request, handler, context, m_executor.get());
}

BatchGetMembershipDatasourcesOutcome DetectiveClient::BatchGetMembershipDatasources(const BatchGetMembershipDatasourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchGetMembershipDatasources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchGetMembershipDatasources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/membership/datasources/get");
  return BatchGetMembershipDatasourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchGetMembershipDatasourcesOutcomeCallable DetectiveClient::BatchGetMembershipDatasourcesCallable(const BatchGetMembershipDatasourcesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::BatchGetMembershipDatasources, this, request, m_executor.get());
}

void DetectiveClient::BatchGetMembershipDatasourcesAsync(const BatchGetMembershipDatasourcesRequest& request, const BatchGetMembershipDatasourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::BatchGetMembershipDatasources, this, request, handler, context, m_executor.get());
}

CreateGraphOutcome DetectiveClient::CreateGraph(const CreateGraphRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGraph, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGraph, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph");
  return CreateGraphOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGraphOutcomeCallable DetectiveClient::CreateGraphCallable(const CreateGraphRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::CreateGraph, this, request, m_executor.get());
}

void DetectiveClient::CreateGraphAsync(const CreateGraphRequest& request, const CreateGraphResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::CreateGraph, this, request, handler, context, m_executor.get());
}

CreateMembersOutcome DetectiveClient::CreateMembers(const CreateMembersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/members");
  return CreateMembersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateMembersOutcomeCallable DetectiveClient::CreateMembersCallable(const CreateMembersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::CreateMembers, this, request, m_executor.get());
}

void DetectiveClient::CreateMembersAsync(const CreateMembersRequest& request, const CreateMembersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::CreateMembers, this, request, handler, context, m_executor.get());
}

DeleteGraphOutcome DetectiveClient::DeleteGraph(const DeleteGraphRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGraph, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGraph, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/removal");
  return DeleteGraphOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteGraphOutcomeCallable DetectiveClient::DeleteGraphCallable(const DeleteGraphRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::DeleteGraph, this, request, m_executor.get());
}

void DetectiveClient::DeleteGraphAsync(const DeleteGraphRequest& request, const DeleteGraphResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::DeleteGraph, this, request, handler, context, m_executor.get());
}

DeleteMembersOutcome DetectiveClient::DeleteMembers(const DeleteMembersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/members/removal");
  return DeleteMembersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteMembersOutcomeCallable DetectiveClient::DeleteMembersCallable(const DeleteMembersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::DeleteMembers, this, request, m_executor.get());
}

void DetectiveClient::DeleteMembersAsync(const DeleteMembersRequest& request, const DeleteMembersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::DeleteMembers, this, request, handler, context, m_executor.get());
}

DescribeOrganizationConfigurationOutcome DetectiveClient::DescribeOrganizationConfiguration(const DescribeOrganizationConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeOrganizationConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeOrganizationConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/orgs/describeOrganizationConfiguration");
  return DescribeOrganizationConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeOrganizationConfigurationOutcomeCallable DetectiveClient::DescribeOrganizationConfigurationCallable(const DescribeOrganizationConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::DescribeOrganizationConfiguration, this, request, m_executor.get());
}

void DetectiveClient::DescribeOrganizationConfigurationAsync(const DescribeOrganizationConfigurationRequest& request, const DescribeOrganizationConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::DescribeOrganizationConfiguration, this, request, handler, context, m_executor.get());
}

DisableOrganizationAdminAccountOutcome DetectiveClient::DisableOrganizationAdminAccount() const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisableOrganizationAdminAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  static const Aws::Vector<Aws::Endpoint::EndpointParameter> staticEndpointParameters;
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(staticEndpointParameters);
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisableOrganizationAdminAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/orgs/disableAdminAccount");
  return DisableOrganizationAdminAccountOutcome(MakeRequest(endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER, "DisableOrganizationAdminAccount"));
}

DisableOrganizationAdminAccountOutcomeCallable DetectiveClient::DisableOrganizationAdminAccountCallable() const
{
  auto task = Aws::MakeShared< std::packaged_task< DisableOrganizationAdminAccountOutcome() > >(ALLOCATION_TAG, [this](){ return this->DisableOrganizationAdminAccount(); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}

void DetectiveClient::DisableOrganizationAdminAccountAsync(const DisableOrganizationAdminAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit( [this, handler, context]()
    {
      handler(this, DisableOrganizationAdminAccount(), context);
    } );
}

DisassociateMembershipOutcome DetectiveClient::DisassociateMembership(const DisassociateMembershipRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateMembership, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/membership/removal");
  return DisassociateMembershipOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateMembershipOutcomeCallable DetectiveClient::DisassociateMembershipCallable(const DisassociateMembershipRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::DisassociateMembership, this, request, m_executor.get());
}

void DetectiveClient::DisassociateMembershipAsync(const DisassociateMembershipRequest& request, const DisassociateMembershipResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::DisassociateMembership, this, request, handler, context, m_executor.get());
}

EnableOrganizationAdminAccountOutcome DetectiveClient::EnableOrganizationAdminAccount(const EnableOrganizationAdminAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EnableOrganizationAdminAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EnableOrganizationAdminAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/orgs/enableAdminAccount");
  return EnableOrganizationAdminAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

EnableOrganizationAdminAccountOutcomeCallable DetectiveClient::EnableOrganizationAdminAccountCallable(const EnableOrganizationAdminAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::EnableOrganizationAdminAccount, this, request, m_executor.get());
}

void DetectiveClient::EnableOrganizationAdminAccountAsync(const EnableOrganizationAdminAccountRequest& request, const EnableOrganizationAdminAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::EnableOrganizationAdminAccount, this, request, handler, context, m_executor.get());
}

GetMembersOutcome DetectiveClient::GetMembers(const GetMembersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/members/get");
  return GetMembersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetMembersOutcomeCallable DetectiveClient::GetMembersCallable(const GetMembersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::GetMembers, this, request, m_executor.get());
}

void DetectiveClient::GetMembersAsync(const GetMembersRequest& request, const GetMembersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::GetMembers, this, request, handler, context, m_executor.get());
}

ListDatasourcePackagesOutcome DetectiveClient::ListDatasourcePackages(const ListDatasourcePackagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDatasourcePackages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDatasourcePackages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/datasources/list");
  return ListDatasourcePackagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListDatasourcePackagesOutcomeCallable DetectiveClient::ListDatasourcePackagesCallable(const ListDatasourcePackagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListDatasourcePackages, this, request, m_executor.get());
}

void DetectiveClient::ListDatasourcePackagesAsync(const ListDatasourcePackagesRequest& request, const ListDatasourcePackagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListDatasourcePackages, this, request, handler, context, m_executor.get());
}

ListGraphsOutcome DetectiveClient::ListGraphs(const ListGraphsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGraphs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGraphs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graphs/list");
  return ListGraphsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGraphsOutcomeCallable DetectiveClient::ListGraphsCallable(const ListGraphsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListGraphs, this, request, m_executor.get());
}

void DetectiveClient::ListGraphsAsync(const ListGraphsRequest& request, const ListGraphsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListGraphs, this, request, handler, context, m_executor.get());
}

ListInvitationsOutcome DetectiveClient::ListInvitations(const ListInvitationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListInvitations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListInvitations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/invitations/list");
  return ListInvitationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListInvitationsOutcomeCallable DetectiveClient::ListInvitationsCallable(const ListInvitationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListInvitations, this, request, m_executor.get());
}

void DetectiveClient::ListInvitationsAsync(const ListInvitationsRequest& request, const ListInvitationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListInvitations, this, request, handler, context, m_executor.get());
}

ListMembersOutcome DetectiveClient::ListMembers(const ListMembersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListMembers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/members/list");
  return ListMembersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListMembersOutcomeCallable DetectiveClient::ListMembersCallable(const ListMembersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListMembers, this, request, m_executor.get());
}

void DetectiveClient::ListMembersAsync(const ListMembersRequest& request, const ListMembersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListMembers, this, request, handler, context, m_executor.get());
}

ListOrganizationAdminAccountsOutcome DetectiveClient::ListOrganizationAdminAccounts(const ListOrganizationAdminAccountsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListOrganizationAdminAccounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListOrganizationAdminAccounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/orgs/adminAccountslist");
  return ListOrganizationAdminAccountsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListOrganizationAdminAccountsOutcomeCallable DetectiveClient::ListOrganizationAdminAccountsCallable(const ListOrganizationAdminAccountsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListOrganizationAdminAccounts, this, request, m_executor.get());
}

void DetectiveClient::ListOrganizationAdminAccountsAsync(const ListOrganizationAdminAccountsRequest& request, const ListOrganizationAdminAccountsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListOrganizationAdminAccounts, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome DetectiveClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<DetectiveErrors>(DetectiveErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable DetectiveClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::ListTagsForResource, this, request, m_executor.get());
}

void DetectiveClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

RejectInvitationOutcome DetectiveClient::RejectInvitation(const RejectInvitationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RejectInvitation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RejectInvitation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/invitation/removal");
  return RejectInvitationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RejectInvitationOutcomeCallable DetectiveClient::RejectInvitationCallable(const RejectInvitationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::RejectInvitation, this, request, m_executor.get());
}

void DetectiveClient::RejectInvitationAsync(const RejectInvitationRequest& request, const RejectInvitationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::RejectInvitation, this, request, handler, context, m_executor.get());
}

StartMonitoringMemberOutcome DetectiveClient::StartMonitoringMember(const StartMonitoringMemberRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartMonitoringMember, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartMonitoringMember, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/member/monitoringstate");
  return StartMonitoringMemberOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartMonitoringMemberOutcomeCallable DetectiveClient::StartMonitoringMemberCallable(const StartMonitoringMemberRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::StartMonitoringMember, this, request, m_executor.get());
}

void DetectiveClient::StartMonitoringMemberAsync(const StartMonitoringMemberRequest& request, const StartMonitoringMemberResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::StartMonitoringMember, this, request, handler, context, m_executor.get());
}

TagResourceOutcome DetectiveClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<DetectiveErrors>(DetectiveErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable DetectiveClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::TagResource, this, request, m_executor.get());
}

void DetectiveClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome DetectiveClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<DetectiveErrors>(DetectiveErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<DetectiveErrors>(DetectiveErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable DetectiveClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::UntagResource, this, request, m_executor.get());
}

void DetectiveClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateDatasourcePackagesOutcome DetectiveClient::UpdateDatasourcePackages(const UpdateDatasourcePackagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDatasourcePackages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDatasourcePackages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/graph/datasources/update");
  return UpdateDatasourcePackagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateDatasourcePackagesOutcomeCallable DetectiveClient::UpdateDatasourcePackagesCallable(const UpdateDatasourcePackagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::UpdateDatasourcePackages, this, request, m_executor.get());
}

void DetectiveClient::UpdateDatasourcePackagesAsync(const UpdateDatasourcePackagesRequest& request, const UpdateDatasourcePackagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::UpdateDatasourcePackages, this, request, handler, context, m_executor.get());
}

UpdateOrganizationConfigurationOutcome DetectiveClient::UpdateOrganizationConfiguration(const UpdateOrganizationConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateOrganizationConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateOrganizationConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/orgs/updateOrganizationConfiguration");
  return UpdateOrganizationConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateOrganizationConfigurationOutcomeCallable DetectiveClient::UpdateOrganizationConfigurationCallable(const UpdateOrganizationConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &DetectiveClient::UpdateOrganizationConfiguration, this, request, m_executor.get());
}

void DetectiveClient::UpdateOrganizationConfigurationAsync(const UpdateOrganizationConfigurationRequest& request, const UpdateOrganizationConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&DetectiveClient::UpdateOrganizationConfiguration, this, request, handler, context, m_executor.get());
}

