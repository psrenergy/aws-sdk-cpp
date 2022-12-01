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

#include <aws/workspaces/WorkSpacesClient.h>
#include <aws/workspaces/WorkSpacesErrorMarshaller.h>
#include <aws/workspaces/WorkSpacesEndpointProvider.h>
#include <aws/workspaces/model/AssociateConnectionAliasRequest.h>
#include <aws/workspaces/model/AssociateIpGroupsRequest.h>
#include <aws/workspaces/model/AuthorizeIpRulesRequest.h>
#include <aws/workspaces/model/CopyWorkspaceImageRequest.h>
#include <aws/workspaces/model/CreateConnectClientAddInRequest.h>
#include <aws/workspaces/model/CreateConnectionAliasRequest.h>
#include <aws/workspaces/model/CreateIpGroupRequest.h>
#include <aws/workspaces/model/CreateStandbyWorkspacesRequest.h>
#include <aws/workspaces/model/CreateTagsRequest.h>
#include <aws/workspaces/model/CreateUpdatedWorkspaceImageRequest.h>
#include <aws/workspaces/model/CreateWorkspaceBundleRequest.h>
#include <aws/workspaces/model/CreateWorkspaceImageRequest.h>
#include <aws/workspaces/model/CreateWorkspacesRequest.h>
#include <aws/workspaces/model/DeleteClientBrandingRequest.h>
#include <aws/workspaces/model/DeleteConnectClientAddInRequest.h>
#include <aws/workspaces/model/DeleteConnectionAliasRequest.h>
#include <aws/workspaces/model/DeleteIpGroupRequest.h>
#include <aws/workspaces/model/DeleteTagsRequest.h>
#include <aws/workspaces/model/DeleteWorkspaceBundleRequest.h>
#include <aws/workspaces/model/DeleteWorkspaceImageRequest.h>
#include <aws/workspaces/model/DeregisterWorkspaceDirectoryRequest.h>
#include <aws/workspaces/model/DescribeAccountRequest.h>
#include <aws/workspaces/model/DescribeAccountModificationsRequest.h>
#include <aws/workspaces/model/DescribeClientBrandingRequest.h>
#include <aws/workspaces/model/DescribeClientPropertiesRequest.h>
#include <aws/workspaces/model/DescribeConnectClientAddInsRequest.h>
#include <aws/workspaces/model/DescribeConnectionAliasPermissionsRequest.h>
#include <aws/workspaces/model/DescribeConnectionAliasesRequest.h>
#include <aws/workspaces/model/DescribeIpGroupsRequest.h>
#include <aws/workspaces/model/DescribeTagsRequest.h>
#include <aws/workspaces/model/DescribeWorkspaceBundlesRequest.h>
#include <aws/workspaces/model/DescribeWorkspaceDirectoriesRequest.h>
#include <aws/workspaces/model/DescribeWorkspaceImagePermissionsRequest.h>
#include <aws/workspaces/model/DescribeWorkspaceImagesRequest.h>
#include <aws/workspaces/model/DescribeWorkspaceSnapshotsRequest.h>
#include <aws/workspaces/model/DescribeWorkspacesRequest.h>
#include <aws/workspaces/model/DescribeWorkspacesConnectionStatusRequest.h>
#include <aws/workspaces/model/DisassociateConnectionAliasRequest.h>
#include <aws/workspaces/model/DisassociateIpGroupsRequest.h>
#include <aws/workspaces/model/ImportClientBrandingRequest.h>
#include <aws/workspaces/model/ImportWorkspaceImageRequest.h>
#include <aws/workspaces/model/ListAvailableManagementCidrRangesRequest.h>
#include <aws/workspaces/model/MigrateWorkspaceRequest.h>
#include <aws/workspaces/model/ModifyAccountRequest.h>
#include <aws/workspaces/model/ModifyCertificateBasedAuthPropertiesRequest.h>
#include <aws/workspaces/model/ModifyClientPropertiesRequest.h>
#include <aws/workspaces/model/ModifySamlPropertiesRequest.h>
#include <aws/workspaces/model/ModifySelfservicePermissionsRequest.h>
#include <aws/workspaces/model/ModifyWorkspaceAccessPropertiesRequest.h>
#include <aws/workspaces/model/ModifyWorkspaceCreationPropertiesRequest.h>
#include <aws/workspaces/model/ModifyWorkspacePropertiesRequest.h>
#include <aws/workspaces/model/ModifyWorkspaceStateRequest.h>
#include <aws/workspaces/model/RebootWorkspacesRequest.h>
#include <aws/workspaces/model/RebuildWorkspacesRequest.h>
#include <aws/workspaces/model/RegisterWorkspaceDirectoryRequest.h>
#include <aws/workspaces/model/RestoreWorkspaceRequest.h>
#include <aws/workspaces/model/RevokeIpRulesRequest.h>
#include <aws/workspaces/model/StartWorkspacesRequest.h>
#include <aws/workspaces/model/StopWorkspacesRequest.h>
#include <aws/workspaces/model/TerminateWorkspacesRequest.h>
#include <aws/workspaces/model/UpdateConnectClientAddInRequest.h>
#include <aws/workspaces/model/UpdateConnectionAliasPermissionRequest.h>
#include <aws/workspaces/model/UpdateRulesOfIpGroupRequest.h>
#include <aws/workspaces/model/UpdateWorkspaceBundleRequest.h>
#include <aws/workspaces/model/UpdateWorkspaceImagePermissionRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::WorkSpaces;
using namespace Aws::WorkSpaces::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* WorkSpacesClient::SERVICE_NAME = "workspaces";
const char* WorkSpacesClient::ALLOCATION_TAG = "WorkSpacesClient";

WorkSpacesClient::WorkSpacesClient(const WorkSpaces::WorkSpacesClientConfiguration& clientConfiguration,
                                   std::shared_ptr<WorkSpacesEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WorkSpacesClient::WorkSpacesClient(const AWSCredentials& credentials,
                                   std::shared_ptr<WorkSpacesEndpointProviderBase> endpointProvider,
                                   const WorkSpaces::WorkSpacesClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

WorkSpacesClient::WorkSpacesClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   std::shared_ptr<WorkSpacesEndpointProviderBase> endpointProvider,
                                   const WorkSpaces::WorkSpacesClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  WorkSpacesClient::WorkSpacesClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<WorkSpacesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WorkSpacesClient::WorkSpacesClient(const AWSCredentials& credentials,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WorkSpacesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

WorkSpacesClient::WorkSpacesClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<WorkSpacesErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<WorkSpacesEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
WorkSpacesClient::~WorkSpacesClient()
{
}

std::shared_ptr<WorkSpacesEndpointProviderBase>& WorkSpacesClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void WorkSpacesClient::init(const WorkSpaces::WorkSpacesClientConfiguration& config)
{
  AWSClient::SetServiceClientName("WorkSpaces");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void WorkSpacesClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateConnectionAliasOutcome WorkSpacesClient::AssociateConnectionAlias(const AssociateConnectionAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateConnectionAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateConnectionAliasOutcomeCallable WorkSpacesClient::AssociateConnectionAliasCallable(const AssociateConnectionAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::AssociateConnectionAlias, this, request, m_executor.get());
}

void WorkSpacesClient::AssociateConnectionAliasAsync(const AssociateConnectionAliasRequest& request, const AssociateConnectionAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::AssociateConnectionAlias, this, request, handler, context, m_executor.get());
}

AssociateIpGroupsOutcome WorkSpacesClient::AssociateIpGroups(const AssociateIpGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateIpGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateIpGroupsOutcomeCallable WorkSpacesClient::AssociateIpGroupsCallable(const AssociateIpGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::AssociateIpGroups, this, request, m_executor.get());
}

void WorkSpacesClient::AssociateIpGroupsAsync(const AssociateIpGroupsRequest& request, const AssociateIpGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::AssociateIpGroups, this, request, handler, context, m_executor.get());
}

AuthorizeIpRulesOutcome WorkSpacesClient::AuthorizeIpRules(const AuthorizeIpRulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeIpRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeIpRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeIpRulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AuthorizeIpRulesOutcomeCallable WorkSpacesClient::AuthorizeIpRulesCallable(const AuthorizeIpRulesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::AuthorizeIpRules, this, request, m_executor.get());
}

void WorkSpacesClient::AuthorizeIpRulesAsync(const AuthorizeIpRulesRequest& request, const AuthorizeIpRulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::AuthorizeIpRules, this, request, handler, context, m_executor.get());
}

CopyWorkspaceImageOutcome WorkSpacesClient::CopyWorkspaceImage(const CopyWorkspaceImageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyWorkspaceImageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CopyWorkspaceImageOutcomeCallable WorkSpacesClient::CopyWorkspaceImageCallable(const CopyWorkspaceImageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CopyWorkspaceImage, this, request, m_executor.get());
}

void WorkSpacesClient::CopyWorkspaceImageAsync(const CopyWorkspaceImageRequest& request, const CopyWorkspaceImageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CopyWorkspaceImage, this, request, handler, context, m_executor.get());
}

CreateConnectClientAddInOutcome WorkSpacesClient::CreateConnectClientAddIn(const CreateConnectClientAddInRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateConnectClientAddInOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConnectClientAddInOutcomeCallable WorkSpacesClient::CreateConnectClientAddInCallable(const CreateConnectClientAddInRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateConnectClientAddIn, this, request, m_executor.get());
}

void WorkSpacesClient::CreateConnectClientAddInAsync(const CreateConnectClientAddInRequest& request, const CreateConnectClientAddInResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateConnectClientAddIn, this, request, handler, context, m_executor.get());
}

CreateConnectionAliasOutcome WorkSpacesClient::CreateConnectionAlias(const CreateConnectionAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateConnectionAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConnectionAliasOutcomeCallable WorkSpacesClient::CreateConnectionAliasCallable(const CreateConnectionAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateConnectionAlias, this, request, m_executor.get());
}

void WorkSpacesClient::CreateConnectionAliasAsync(const CreateConnectionAliasRequest& request, const CreateConnectionAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateConnectionAlias, this, request, handler, context, m_executor.get());
}

CreateIpGroupOutcome WorkSpacesClient::CreateIpGroup(const CreateIpGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateIpGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateIpGroupOutcomeCallable WorkSpacesClient::CreateIpGroupCallable(const CreateIpGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateIpGroup, this, request, m_executor.get());
}

void WorkSpacesClient::CreateIpGroupAsync(const CreateIpGroupRequest& request, const CreateIpGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateIpGroup, this, request, handler, context, m_executor.get());
}

CreateStandbyWorkspacesOutcome WorkSpacesClient::CreateStandbyWorkspaces(const CreateStandbyWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateStandbyWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateStandbyWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateStandbyWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateStandbyWorkspacesOutcomeCallable WorkSpacesClient::CreateStandbyWorkspacesCallable(const CreateStandbyWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateStandbyWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::CreateStandbyWorkspacesAsync(const CreateStandbyWorkspacesRequest& request, const CreateStandbyWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateStandbyWorkspaces, this, request, handler, context, m_executor.get());
}

CreateTagsOutcome WorkSpacesClient::CreateTags(const CreateTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateTagsOutcomeCallable WorkSpacesClient::CreateTagsCallable(const CreateTagsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateTags, this, request, m_executor.get());
}

void WorkSpacesClient::CreateTagsAsync(const CreateTagsRequest& request, const CreateTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateTags, this, request, handler, context, m_executor.get());
}

CreateUpdatedWorkspaceImageOutcome WorkSpacesClient::CreateUpdatedWorkspaceImage(const CreateUpdatedWorkspaceImageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUpdatedWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUpdatedWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUpdatedWorkspaceImageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUpdatedWorkspaceImageOutcomeCallable WorkSpacesClient::CreateUpdatedWorkspaceImageCallable(const CreateUpdatedWorkspaceImageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateUpdatedWorkspaceImage, this, request, m_executor.get());
}

void WorkSpacesClient::CreateUpdatedWorkspaceImageAsync(const CreateUpdatedWorkspaceImageRequest& request, const CreateUpdatedWorkspaceImageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateUpdatedWorkspaceImage, this, request, handler, context, m_executor.get());
}

CreateWorkspaceBundleOutcome WorkSpacesClient::CreateWorkspaceBundle(const CreateWorkspaceBundleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWorkspaceBundleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorkspaceBundleOutcomeCallable WorkSpacesClient::CreateWorkspaceBundleCallable(const CreateWorkspaceBundleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateWorkspaceBundle, this, request, m_executor.get());
}

void WorkSpacesClient::CreateWorkspaceBundleAsync(const CreateWorkspaceBundleRequest& request, const CreateWorkspaceBundleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateWorkspaceBundle, this, request, handler, context, m_executor.get());
}

CreateWorkspaceImageOutcome WorkSpacesClient::CreateWorkspaceImage(const CreateWorkspaceImageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWorkspaceImageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorkspaceImageOutcomeCallable WorkSpacesClient::CreateWorkspaceImageCallable(const CreateWorkspaceImageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateWorkspaceImage, this, request, m_executor.get());
}

void WorkSpacesClient::CreateWorkspaceImageAsync(const CreateWorkspaceImageRequest& request, const CreateWorkspaceImageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateWorkspaceImage, this, request, handler, context, m_executor.get());
}

CreateWorkspacesOutcome WorkSpacesClient::CreateWorkspaces(const CreateWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorkspacesOutcomeCallable WorkSpacesClient::CreateWorkspacesCallable(const CreateWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::CreateWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::CreateWorkspacesAsync(const CreateWorkspacesRequest& request, const CreateWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::CreateWorkspaces, this, request, handler, context, m_executor.get());
}

DeleteClientBrandingOutcome WorkSpacesClient::DeleteClientBranding(const DeleteClientBrandingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClientBrandingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteClientBrandingOutcomeCallable WorkSpacesClient::DeleteClientBrandingCallable(const DeleteClientBrandingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteClientBranding, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteClientBrandingAsync(const DeleteClientBrandingRequest& request, const DeleteClientBrandingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteClientBranding, this, request, handler, context, m_executor.get());
}

DeleteConnectClientAddInOutcome WorkSpacesClient::DeleteConnectClientAddIn(const DeleteConnectClientAddInRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteConnectClientAddInOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteConnectClientAddInOutcomeCallable WorkSpacesClient::DeleteConnectClientAddInCallable(const DeleteConnectClientAddInRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteConnectClientAddIn, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteConnectClientAddInAsync(const DeleteConnectClientAddInRequest& request, const DeleteConnectClientAddInResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteConnectClientAddIn, this, request, handler, context, m_executor.get());
}

DeleteConnectionAliasOutcome WorkSpacesClient::DeleteConnectionAlias(const DeleteConnectionAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteConnectionAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteConnectionAliasOutcomeCallable WorkSpacesClient::DeleteConnectionAliasCallable(const DeleteConnectionAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteConnectionAlias, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteConnectionAliasAsync(const DeleteConnectionAliasRequest& request, const DeleteConnectionAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteConnectionAlias, this, request, handler, context, m_executor.get());
}

DeleteIpGroupOutcome WorkSpacesClient::DeleteIpGroup(const DeleteIpGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteIpGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteIpGroupOutcomeCallable WorkSpacesClient::DeleteIpGroupCallable(const DeleteIpGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteIpGroup, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteIpGroupAsync(const DeleteIpGroupRequest& request, const DeleteIpGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteIpGroup, this, request, handler, context, m_executor.get());
}

DeleteTagsOutcome WorkSpacesClient::DeleteTags(const DeleteTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteTagsOutcomeCallable WorkSpacesClient::DeleteTagsCallable(const DeleteTagsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteTags, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteTagsAsync(const DeleteTagsRequest& request, const DeleteTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteTags, this, request, handler, context, m_executor.get());
}

DeleteWorkspaceBundleOutcome WorkSpacesClient::DeleteWorkspaceBundle(const DeleteWorkspaceBundleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteWorkspaceBundleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteWorkspaceBundleOutcomeCallable WorkSpacesClient::DeleteWorkspaceBundleCallable(const DeleteWorkspaceBundleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteWorkspaceBundle, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteWorkspaceBundleAsync(const DeleteWorkspaceBundleRequest& request, const DeleteWorkspaceBundleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteWorkspaceBundle, this, request, handler, context, m_executor.get());
}

DeleteWorkspaceImageOutcome WorkSpacesClient::DeleteWorkspaceImage(const DeleteWorkspaceImageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteWorkspaceImageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteWorkspaceImageOutcomeCallable WorkSpacesClient::DeleteWorkspaceImageCallable(const DeleteWorkspaceImageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeleteWorkspaceImage, this, request, m_executor.get());
}

void WorkSpacesClient::DeleteWorkspaceImageAsync(const DeleteWorkspaceImageRequest& request, const DeleteWorkspaceImageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeleteWorkspaceImage, this, request, handler, context, m_executor.get());
}

DeregisterWorkspaceDirectoryOutcome WorkSpacesClient::DeregisterWorkspaceDirectory(const DeregisterWorkspaceDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeregisterWorkspaceDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeregisterWorkspaceDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeregisterWorkspaceDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeregisterWorkspaceDirectoryOutcomeCallable WorkSpacesClient::DeregisterWorkspaceDirectoryCallable(const DeregisterWorkspaceDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DeregisterWorkspaceDirectory, this, request, m_executor.get());
}

void WorkSpacesClient::DeregisterWorkspaceDirectoryAsync(const DeregisterWorkspaceDirectoryRequest& request, const DeregisterWorkspaceDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DeregisterWorkspaceDirectory, this, request, handler, context, m_executor.get());
}

DescribeAccountOutcome WorkSpacesClient::DescribeAccount(const DescribeAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccountOutcomeCallable WorkSpacesClient::DescribeAccountCallable(const DescribeAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeAccount, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeAccountAsync(const DescribeAccountRequest& request, const DescribeAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeAccount, this, request, handler, context, m_executor.get());
}

DescribeAccountModificationsOutcome WorkSpacesClient::DescribeAccountModifications(const DescribeAccountModificationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountModifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountModifications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountModificationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccountModificationsOutcomeCallable WorkSpacesClient::DescribeAccountModificationsCallable(const DescribeAccountModificationsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeAccountModifications, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeAccountModificationsAsync(const DescribeAccountModificationsRequest& request, const DescribeAccountModificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeAccountModifications, this, request, handler, context, m_executor.get());
}

DescribeClientBrandingOutcome WorkSpacesClient::DescribeClientBranding(const DescribeClientBrandingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClientBrandingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeClientBrandingOutcomeCallable WorkSpacesClient::DescribeClientBrandingCallable(const DescribeClientBrandingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeClientBranding, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeClientBrandingAsync(const DescribeClientBrandingRequest& request, const DescribeClientBrandingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeClientBranding, this, request, handler, context, m_executor.get());
}

DescribeClientPropertiesOutcome WorkSpacesClient::DescribeClientProperties(const DescribeClientPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClientProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClientProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClientPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeClientPropertiesOutcomeCallable WorkSpacesClient::DescribeClientPropertiesCallable(const DescribeClientPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeClientProperties, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeClientPropertiesAsync(const DescribeClientPropertiesRequest& request, const DescribeClientPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeClientProperties, this, request, handler, context, m_executor.get());
}

DescribeConnectClientAddInsOutcome WorkSpacesClient::DescribeConnectClientAddIns(const DescribeConnectClientAddInsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeConnectClientAddIns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeConnectClientAddIns, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeConnectClientAddInsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeConnectClientAddInsOutcomeCallable WorkSpacesClient::DescribeConnectClientAddInsCallable(const DescribeConnectClientAddInsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeConnectClientAddIns, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeConnectClientAddInsAsync(const DescribeConnectClientAddInsRequest& request, const DescribeConnectClientAddInsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeConnectClientAddIns, this, request, handler, context, m_executor.get());
}

DescribeConnectionAliasPermissionsOutcome WorkSpacesClient::DescribeConnectionAliasPermissions(const DescribeConnectionAliasPermissionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeConnectionAliasPermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeConnectionAliasPermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeConnectionAliasPermissionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeConnectionAliasPermissionsOutcomeCallable WorkSpacesClient::DescribeConnectionAliasPermissionsCallable(const DescribeConnectionAliasPermissionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeConnectionAliasPermissions, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeConnectionAliasPermissionsAsync(const DescribeConnectionAliasPermissionsRequest& request, const DescribeConnectionAliasPermissionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeConnectionAliasPermissions, this, request, handler, context, m_executor.get());
}

DescribeConnectionAliasesOutcome WorkSpacesClient::DescribeConnectionAliases(const DescribeConnectionAliasesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeConnectionAliases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeConnectionAliases, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeConnectionAliasesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeConnectionAliasesOutcomeCallable WorkSpacesClient::DescribeConnectionAliasesCallable(const DescribeConnectionAliasesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeConnectionAliases, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeConnectionAliasesAsync(const DescribeConnectionAliasesRequest& request, const DescribeConnectionAliasesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeConnectionAliases, this, request, handler, context, m_executor.get());
}

DescribeIpGroupsOutcome WorkSpacesClient::DescribeIpGroups(const DescribeIpGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeIpGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeIpGroupsOutcomeCallable WorkSpacesClient::DescribeIpGroupsCallable(const DescribeIpGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeIpGroups, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeIpGroupsAsync(const DescribeIpGroupsRequest& request, const DescribeIpGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeIpGroups, this, request, handler, context, m_executor.get());
}

DescribeTagsOutcome WorkSpacesClient::DescribeTags(const DescribeTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeTagsOutcomeCallable WorkSpacesClient::DescribeTagsCallable(const DescribeTagsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeTags, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeTagsAsync(const DescribeTagsRequest& request, const DescribeTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeTags, this, request, handler, context, m_executor.get());
}

DescribeWorkspaceBundlesOutcome WorkSpacesClient::DescribeWorkspaceBundles(const DescribeWorkspaceBundlesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaceBundles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaceBundles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspaceBundlesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspaceBundlesOutcomeCallable WorkSpacesClient::DescribeWorkspaceBundlesCallable(const DescribeWorkspaceBundlesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaceBundles, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspaceBundlesAsync(const DescribeWorkspaceBundlesRequest& request, const DescribeWorkspaceBundlesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaceBundles, this, request, handler, context, m_executor.get());
}

DescribeWorkspaceDirectoriesOutcome WorkSpacesClient::DescribeWorkspaceDirectories(const DescribeWorkspaceDirectoriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaceDirectories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaceDirectories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspaceDirectoriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspaceDirectoriesOutcomeCallable WorkSpacesClient::DescribeWorkspaceDirectoriesCallable(const DescribeWorkspaceDirectoriesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaceDirectories, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspaceDirectoriesAsync(const DescribeWorkspaceDirectoriesRequest& request, const DescribeWorkspaceDirectoriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaceDirectories, this, request, handler, context, m_executor.get());
}

DescribeWorkspaceImagePermissionsOutcome WorkSpacesClient::DescribeWorkspaceImagePermissions(const DescribeWorkspaceImagePermissionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaceImagePermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaceImagePermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspaceImagePermissionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspaceImagePermissionsOutcomeCallable WorkSpacesClient::DescribeWorkspaceImagePermissionsCallable(const DescribeWorkspaceImagePermissionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaceImagePermissions, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspaceImagePermissionsAsync(const DescribeWorkspaceImagePermissionsRequest& request, const DescribeWorkspaceImagePermissionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaceImagePermissions, this, request, handler, context, m_executor.get());
}

DescribeWorkspaceImagesOutcome WorkSpacesClient::DescribeWorkspaceImages(const DescribeWorkspaceImagesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaceImages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaceImages, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspaceImagesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspaceImagesOutcomeCallable WorkSpacesClient::DescribeWorkspaceImagesCallable(const DescribeWorkspaceImagesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaceImages, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspaceImagesAsync(const DescribeWorkspaceImagesRequest& request, const DescribeWorkspaceImagesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaceImages, this, request, handler, context, m_executor.get());
}

DescribeWorkspaceSnapshotsOutcome WorkSpacesClient::DescribeWorkspaceSnapshots(const DescribeWorkspaceSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaceSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaceSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspaceSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspaceSnapshotsOutcomeCallable WorkSpacesClient::DescribeWorkspaceSnapshotsCallable(const DescribeWorkspaceSnapshotsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaceSnapshots, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspaceSnapshotsAsync(const DescribeWorkspaceSnapshotsRequest& request, const DescribeWorkspaceSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaceSnapshots, this, request, handler, context, m_executor.get());
}

DescribeWorkspacesOutcome WorkSpacesClient::DescribeWorkspaces(const DescribeWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspacesOutcomeCallable WorkSpacesClient::DescribeWorkspacesCallable(const DescribeWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspacesAsync(const DescribeWorkspacesRequest& request, const DescribeWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspaces, this, request, handler, context, m_executor.get());
}

DescribeWorkspacesConnectionStatusOutcome WorkSpacesClient::DescribeWorkspacesConnectionStatus(const DescribeWorkspacesConnectionStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorkspacesConnectionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorkspacesConnectionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeWorkspacesConnectionStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorkspacesConnectionStatusOutcomeCallable WorkSpacesClient::DescribeWorkspacesConnectionStatusCallable(const DescribeWorkspacesConnectionStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DescribeWorkspacesConnectionStatus, this, request, m_executor.get());
}

void WorkSpacesClient::DescribeWorkspacesConnectionStatusAsync(const DescribeWorkspacesConnectionStatusRequest& request, const DescribeWorkspacesConnectionStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DescribeWorkspacesConnectionStatus, this, request, handler, context, m_executor.get());
}

DisassociateConnectionAliasOutcome WorkSpacesClient::DisassociateConnectionAlias(const DisassociateConnectionAliasRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateConnectionAlias, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateConnectionAliasOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateConnectionAliasOutcomeCallable WorkSpacesClient::DisassociateConnectionAliasCallable(const DisassociateConnectionAliasRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DisassociateConnectionAlias, this, request, m_executor.get());
}

void WorkSpacesClient::DisassociateConnectionAliasAsync(const DisassociateConnectionAliasRequest& request, const DisassociateConnectionAliasResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DisassociateConnectionAlias, this, request, handler, context, m_executor.get());
}

DisassociateIpGroupsOutcome WorkSpacesClient::DisassociateIpGroups(const DisassociateIpGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateIpGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateIpGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateIpGroupsOutcomeCallable WorkSpacesClient::DisassociateIpGroupsCallable(const DisassociateIpGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::DisassociateIpGroups, this, request, m_executor.get());
}

void WorkSpacesClient::DisassociateIpGroupsAsync(const DisassociateIpGroupsRequest& request, const DisassociateIpGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::DisassociateIpGroups, this, request, handler, context, m_executor.get());
}

ImportClientBrandingOutcome WorkSpacesClient::ImportClientBranding(const ImportClientBrandingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportClientBranding, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ImportClientBrandingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportClientBrandingOutcomeCallable WorkSpacesClient::ImportClientBrandingCallable(const ImportClientBrandingRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ImportClientBranding, this, request, m_executor.get());
}

void WorkSpacesClient::ImportClientBrandingAsync(const ImportClientBrandingRequest& request, const ImportClientBrandingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ImportClientBranding, this, request, handler, context, m_executor.get());
}

ImportWorkspaceImageOutcome WorkSpacesClient::ImportWorkspaceImage(const ImportWorkspaceImageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ImportWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ImportWorkspaceImage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ImportWorkspaceImageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ImportWorkspaceImageOutcomeCallable WorkSpacesClient::ImportWorkspaceImageCallable(const ImportWorkspaceImageRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ImportWorkspaceImage, this, request, m_executor.get());
}

void WorkSpacesClient::ImportWorkspaceImageAsync(const ImportWorkspaceImageRequest& request, const ImportWorkspaceImageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ImportWorkspaceImage, this, request, handler, context, m_executor.get());
}

ListAvailableManagementCidrRangesOutcome WorkSpacesClient::ListAvailableManagementCidrRanges(const ListAvailableManagementCidrRangesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAvailableManagementCidrRanges, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAvailableManagementCidrRanges, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAvailableManagementCidrRangesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAvailableManagementCidrRangesOutcomeCallable WorkSpacesClient::ListAvailableManagementCidrRangesCallable(const ListAvailableManagementCidrRangesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ListAvailableManagementCidrRanges, this, request, m_executor.get());
}

void WorkSpacesClient::ListAvailableManagementCidrRangesAsync(const ListAvailableManagementCidrRangesRequest& request, const ListAvailableManagementCidrRangesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ListAvailableManagementCidrRanges, this, request, handler, context, m_executor.get());
}

MigrateWorkspaceOutcome WorkSpacesClient::MigrateWorkspace(const MigrateWorkspaceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, MigrateWorkspace, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, MigrateWorkspace, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return MigrateWorkspaceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

MigrateWorkspaceOutcomeCallable WorkSpacesClient::MigrateWorkspaceCallable(const MigrateWorkspaceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::MigrateWorkspace, this, request, m_executor.get());
}

void WorkSpacesClient::MigrateWorkspaceAsync(const MigrateWorkspaceRequest& request, const MigrateWorkspaceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::MigrateWorkspace, this, request, handler, context, m_executor.get());
}

ModifyAccountOutcome WorkSpacesClient::ModifyAccount(const ModifyAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyAccountOutcomeCallable WorkSpacesClient::ModifyAccountCallable(const ModifyAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyAccount, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyAccountAsync(const ModifyAccountRequest& request, const ModifyAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyAccount, this, request, handler, context, m_executor.get());
}

ModifyCertificateBasedAuthPropertiesOutcome WorkSpacesClient::ModifyCertificateBasedAuthProperties(const ModifyCertificateBasedAuthPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyCertificateBasedAuthProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyCertificateBasedAuthProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyCertificateBasedAuthPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyCertificateBasedAuthPropertiesOutcomeCallable WorkSpacesClient::ModifyCertificateBasedAuthPropertiesCallable(const ModifyCertificateBasedAuthPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyCertificateBasedAuthProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyCertificateBasedAuthPropertiesAsync(const ModifyCertificateBasedAuthPropertiesRequest& request, const ModifyCertificateBasedAuthPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyCertificateBasedAuthProperties, this, request, handler, context, m_executor.get());
}

ModifyClientPropertiesOutcome WorkSpacesClient::ModifyClientProperties(const ModifyClientPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClientProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClientProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClientPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyClientPropertiesOutcomeCallable WorkSpacesClient::ModifyClientPropertiesCallable(const ModifyClientPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyClientProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyClientPropertiesAsync(const ModifyClientPropertiesRequest& request, const ModifyClientPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyClientProperties, this, request, handler, context, m_executor.get());
}

ModifySamlPropertiesOutcome WorkSpacesClient::ModifySamlProperties(const ModifySamlPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifySamlProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifySamlProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifySamlPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifySamlPropertiesOutcomeCallable WorkSpacesClient::ModifySamlPropertiesCallable(const ModifySamlPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifySamlProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifySamlPropertiesAsync(const ModifySamlPropertiesRequest& request, const ModifySamlPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifySamlProperties, this, request, handler, context, m_executor.get());
}

ModifySelfservicePermissionsOutcome WorkSpacesClient::ModifySelfservicePermissions(const ModifySelfservicePermissionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifySelfservicePermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifySelfservicePermissions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifySelfservicePermissionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifySelfservicePermissionsOutcomeCallable WorkSpacesClient::ModifySelfservicePermissionsCallable(const ModifySelfservicePermissionsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifySelfservicePermissions, this, request, m_executor.get());
}

void WorkSpacesClient::ModifySelfservicePermissionsAsync(const ModifySelfservicePermissionsRequest& request, const ModifySelfservicePermissionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifySelfservicePermissions, this, request, handler, context, m_executor.get());
}

ModifyWorkspaceAccessPropertiesOutcome WorkSpacesClient::ModifyWorkspaceAccessProperties(const ModifyWorkspaceAccessPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyWorkspaceAccessProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyWorkspaceAccessProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyWorkspaceAccessPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyWorkspaceAccessPropertiesOutcomeCallable WorkSpacesClient::ModifyWorkspaceAccessPropertiesCallable(const ModifyWorkspaceAccessPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyWorkspaceAccessProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyWorkspaceAccessPropertiesAsync(const ModifyWorkspaceAccessPropertiesRequest& request, const ModifyWorkspaceAccessPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyWorkspaceAccessProperties, this, request, handler, context, m_executor.get());
}

ModifyWorkspaceCreationPropertiesOutcome WorkSpacesClient::ModifyWorkspaceCreationProperties(const ModifyWorkspaceCreationPropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyWorkspaceCreationProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyWorkspaceCreationProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyWorkspaceCreationPropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyWorkspaceCreationPropertiesOutcomeCallable WorkSpacesClient::ModifyWorkspaceCreationPropertiesCallable(const ModifyWorkspaceCreationPropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyWorkspaceCreationProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyWorkspaceCreationPropertiesAsync(const ModifyWorkspaceCreationPropertiesRequest& request, const ModifyWorkspaceCreationPropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyWorkspaceCreationProperties, this, request, handler, context, m_executor.get());
}

ModifyWorkspacePropertiesOutcome WorkSpacesClient::ModifyWorkspaceProperties(const ModifyWorkspacePropertiesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyWorkspaceProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyWorkspaceProperties, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyWorkspacePropertiesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyWorkspacePropertiesOutcomeCallable WorkSpacesClient::ModifyWorkspacePropertiesCallable(const ModifyWorkspacePropertiesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyWorkspaceProperties, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyWorkspacePropertiesAsync(const ModifyWorkspacePropertiesRequest& request, const ModifyWorkspacePropertiesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyWorkspaceProperties, this, request, handler, context, m_executor.get());
}

ModifyWorkspaceStateOutcome WorkSpacesClient::ModifyWorkspaceState(const ModifyWorkspaceStateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyWorkspaceState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyWorkspaceState, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyWorkspaceStateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ModifyWorkspaceStateOutcomeCallable WorkSpacesClient::ModifyWorkspaceStateCallable(const ModifyWorkspaceStateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::ModifyWorkspaceState, this, request, m_executor.get());
}

void WorkSpacesClient::ModifyWorkspaceStateAsync(const ModifyWorkspaceStateRequest& request, const ModifyWorkspaceStateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::ModifyWorkspaceState, this, request, handler, context, m_executor.get());
}

RebootWorkspacesOutcome WorkSpacesClient::RebootWorkspaces(const RebootWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RebootWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RebootWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RebootWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RebootWorkspacesOutcomeCallable WorkSpacesClient::RebootWorkspacesCallable(const RebootWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::RebootWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::RebootWorkspacesAsync(const RebootWorkspacesRequest& request, const RebootWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::RebootWorkspaces, this, request, handler, context, m_executor.get());
}

RebuildWorkspacesOutcome WorkSpacesClient::RebuildWorkspaces(const RebuildWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RebuildWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RebuildWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RebuildWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RebuildWorkspacesOutcomeCallable WorkSpacesClient::RebuildWorkspacesCallable(const RebuildWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::RebuildWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::RebuildWorkspacesAsync(const RebuildWorkspacesRequest& request, const RebuildWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::RebuildWorkspaces, this, request, handler, context, m_executor.get());
}

RegisterWorkspaceDirectoryOutcome WorkSpacesClient::RegisterWorkspaceDirectory(const RegisterWorkspaceDirectoryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterWorkspaceDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterWorkspaceDirectory, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RegisterWorkspaceDirectoryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RegisterWorkspaceDirectoryOutcomeCallable WorkSpacesClient::RegisterWorkspaceDirectoryCallable(const RegisterWorkspaceDirectoryRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::RegisterWorkspaceDirectory, this, request, m_executor.get());
}

void WorkSpacesClient::RegisterWorkspaceDirectoryAsync(const RegisterWorkspaceDirectoryRequest& request, const RegisterWorkspaceDirectoryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::RegisterWorkspaceDirectory, this, request, handler, context, m_executor.get());
}

RestoreWorkspaceOutcome WorkSpacesClient::RestoreWorkspace(const RestoreWorkspaceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreWorkspace, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreWorkspace, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreWorkspaceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RestoreWorkspaceOutcomeCallable WorkSpacesClient::RestoreWorkspaceCallable(const RestoreWorkspaceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::RestoreWorkspace, this, request, m_executor.get());
}

void WorkSpacesClient::RestoreWorkspaceAsync(const RestoreWorkspaceRequest& request, const RestoreWorkspaceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::RestoreWorkspace, this, request, handler, context, m_executor.get());
}

RevokeIpRulesOutcome WorkSpacesClient::RevokeIpRules(const RevokeIpRulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeIpRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeIpRules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeIpRulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RevokeIpRulesOutcomeCallable WorkSpacesClient::RevokeIpRulesCallable(const RevokeIpRulesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::RevokeIpRules, this, request, m_executor.get());
}

void WorkSpacesClient::RevokeIpRulesAsync(const RevokeIpRulesRequest& request, const RevokeIpRulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::RevokeIpRules, this, request, handler, context, m_executor.get());
}

StartWorkspacesOutcome WorkSpacesClient::StartWorkspaces(const StartWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartWorkspacesOutcomeCallable WorkSpacesClient::StartWorkspacesCallable(const StartWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::StartWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::StartWorkspacesAsync(const StartWorkspacesRequest& request, const StartWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::StartWorkspaces, this, request, handler, context, m_executor.get());
}

StopWorkspacesOutcome WorkSpacesClient::StopWorkspaces(const StopWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopWorkspacesOutcomeCallable WorkSpacesClient::StopWorkspacesCallable(const StopWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::StopWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::StopWorkspacesAsync(const StopWorkspacesRequest& request, const StopWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::StopWorkspaces, this, request, handler, context, m_executor.get());
}

TerminateWorkspacesOutcome WorkSpacesClient::TerminateWorkspaces(const TerminateWorkspacesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TerminateWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TerminateWorkspaces, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TerminateWorkspacesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TerminateWorkspacesOutcomeCallable WorkSpacesClient::TerminateWorkspacesCallable(const TerminateWorkspacesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::TerminateWorkspaces, this, request, m_executor.get());
}

void WorkSpacesClient::TerminateWorkspacesAsync(const TerminateWorkspacesRequest& request, const TerminateWorkspacesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::TerminateWorkspaces, this, request, handler, context, m_executor.get());
}

UpdateConnectClientAddInOutcome WorkSpacesClient::UpdateConnectClientAddIn(const UpdateConnectClientAddInRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateConnectClientAddIn, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateConnectClientAddInOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateConnectClientAddInOutcomeCallable WorkSpacesClient::UpdateConnectClientAddInCallable(const UpdateConnectClientAddInRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::UpdateConnectClientAddIn, this, request, m_executor.get());
}

void WorkSpacesClient::UpdateConnectClientAddInAsync(const UpdateConnectClientAddInRequest& request, const UpdateConnectClientAddInResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::UpdateConnectClientAddIn, this, request, handler, context, m_executor.get());
}

UpdateConnectionAliasPermissionOutcome WorkSpacesClient::UpdateConnectionAliasPermission(const UpdateConnectionAliasPermissionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateConnectionAliasPermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateConnectionAliasPermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateConnectionAliasPermissionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateConnectionAliasPermissionOutcomeCallable WorkSpacesClient::UpdateConnectionAliasPermissionCallable(const UpdateConnectionAliasPermissionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::UpdateConnectionAliasPermission, this, request, m_executor.get());
}

void WorkSpacesClient::UpdateConnectionAliasPermissionAsync(const UpdateConnectionAliasPermissionRequest& request, const UpdateConnectionAliasPermissionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::UpdateConnectionAliasPermission, this, request, handler, context, m_executor.get());
}

UpdateRulesOfIpGroupOutcome WorkSpacesClient::UpdateRulesOfIpGroup(const UpdateRulesOfIpGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRulesOfIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRulesOfIpGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateRulesOfIpGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRulesOfIpGroupOutcomeCallable WorkSpacesClient::UpdateRulesOfIpGroupCallable(const UpdateRulesOfIpGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::UpdateRulesOfIpGroup, this, request, m_executor.get());
}

void WorkSpacesClient::UpdateRulesOfIpGroupAsync(const UpdateRulesOfIpGroupRequest& request, const UpdateRulesOfIpGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::UpdateRulesOfIpGroup, this, request, handler, context, m_executor.get());
}

UpdateWorkspaceBundleOutcome WorkSpacesClient::UpdateWorkspaceBundle(const UpdateWorkspaceBundleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateWorkspaceBundle, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateWorkspaceBundleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateWorkspaceBundleOutcomeCallable WorkSpacesClient::UpdateWorkspaceBundleCallable(const UpdateWorkspaceBundleRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::UpdateWorkspaceBundle, this, request, m_executor.get());
}

void WorkSpacesClient::UpdateWorkspaceBundleAsync(const UpdateWorkspaceBundleRequest& request, const UpdateWorkspaceBundleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::UpdateWorkspaceBundle, this, request, handler, context, m_executor.get());
}

UpdateWorkspaceImagePermissionOutcome WorkSpacesClient::UpdateWorkspaceImagePermission(const UpdateWorkspaceImagePermissionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateWorkspaceImagePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateWorkspaceImagePermission, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateWorkspaceImagePermissionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateWorkspaceImagePermissionOutcomeCallable WorkSpacesClient::UpdateWorkspaceImagePermissionCallable(const UpdateWorkspaceImagePermissionRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &WorkSpacesClient::UpdateWorkspaceImagePermission, this, request, m_executor.get());
}

void WorkSpacesClient::UpdateWorkspaceImagePermissionAsync(const UpdateWorkspaceImagePermissionRequest& request, const UpdateWorkspaceImagePermissionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&WorkSpacesClient::UpdateWorkspaceImagePermission, this, request, handler, context, m_executor.get());
}

