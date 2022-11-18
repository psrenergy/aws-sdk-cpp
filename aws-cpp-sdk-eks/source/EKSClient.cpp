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

#include <aws/eks/EKSClient.h>
#include <aws/eks/EKSErrorMarshaller.h>
#include <aws/eks/EKSEndpointProvider.h>
#include <aws/eks/model/AssociateEncryptionConfigRequest.h>
#include <aws/eks/model/AssociateIdentityProviderConfigRequest.h>
#include <aws/eks/model/CreateAddonRequest.h>
#include <aws/eks/model/CreateClusterRequest.h>
#include <aws/eks/model/CreateFargateProfileRequest.h>
#include <aws/eks/model/CreateNodegroupRequest.h>
#include <aws/eks/model/DeleteAddonRequest.h>
#include <aws/eks/model/DeleteClusterRequest.h>
#include <aws/eks/model/DeleteFargateProfileRequest.h>
#include <aws/eks/model/DeleteNodegroupRequest.h>
#include <aws/eks/model/DeregisterClusterRequest.h>
#include <aws/eks/model/DescribeAddonRequest.h>
#include <aws/eks/model/DescribeAddonVersionsRequest.h>
#include <aws/eks/model/DescribeClusterRequest.h>
#include <aws/eks/model/DescribeFargateProfileRequest.h>
#include <aws/eks/model/DescribeIdentityProviderConfigRequest.h>
#include <aws/eks/model/DescribeNodegroupRequest.h>
#include <aws/eks/model/DescribeUpdateRequest.h>
#include <aws/eks/model/DisassociateIdentityProviderConfigRequest.h>
#include <aws/eks/model/ListAddonsRequest.h>
#include <aws/eks/model/ListClustersRequest.h>
#include <aws/eks/model/ListFargateProfilesRequest.h>
#include <aws/eks/model/ListIdentityProviderConfigsRequest.h>
#include <aws/eks/model/ListNodegroupsRequest.h>
#include <aws/eks/model/ListTagsForResourceRequest.h>
#include <aws/eks/model/ListUpdatesRequest.h>
#include <aws/eks/model/RegisterClusterRequest.h>
#include <aws/eks/model/TagResourceRequest.h>
#include <aws/eks/model/UntagResourceRequest.h>
#include <aws/eks/model/UpdateAddonRequest.h>
#include <aws/eks/model/UpdateClusterConfigRequest.h>
#include <aws/eks/model/UpdateClusterVersionRequest.h>
#include <aws/eks/model/UpdateNodegroupConfigRequest.h>
#include <aws/eks/model/UpdateNodegroupVersionRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::EKS;
using namespace Aws::EKS::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* EKSClient::SERVICE_NAME = "eks";
const char* EKSClient::ALLOCATION_TAG = "EKSClient";

EKSClient::EKSClient(const EKS::EKSClientConfiguration& clientConfiguration,
                     std::shared_ptr<EKSEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

EKSClient::EKSClient(const AWSCredentials& credentials,
                     std::shared_ptr<EKSEndpointProviderBase> endpointProvider,
                     const EKS::EKSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

EKSClient::EKSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     std::shared_ptr<EKSEndpointProviderBase> endpointProvider,
                     const EKS::EKSClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  EKSClient::EKSClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<EKSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

EKSClient::EKSClient(const AWSCredentials& credentials,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<EKSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

EKSClient::EKSClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<EKSErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<EKSEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
EKSClient::~EKSClient()
{
}

std::shared_ptr<EKSEndpointProviderBase>& EKSClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void EKSClient::init(const EKS::EKSClientConfiguration& config)
{
  AWSClient::SetServiceClientName("EKS");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void EKSClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateEncryptionConfigOutcome EKSClient::AssociateEncryptionConfig(const AssociateEncryptionConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateEncryptionConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateEncryptionConfig", "Required field: ClusterName, is not set");
    return AssociateEncryptionConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateEncryptionConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/encryption-config/associate");
  return AssociateEncryptionConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateEncryptionConfigOutcomeCallable EKSClient::AssociateEncryptionConfigCallable(const AssociateEncryptionConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateEncryptionConfig, request, m_executor.get());
}

void EKSClient::AssociateEncryptionConfigAsync(const AssociateEncryptionConfigRequest& request, const AssociateEncryptionConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateEncryptionConfig, request, handler, context, m_executor.get());
}

AssociateIdentityProviderConfigOutcome EKSClient::AssociateIdentityProviderConfig(const AssociateIdentityProviderConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateIdentityProviderConfig", "Required field: ClusterName, is not set");
    return AssociateIdentityProviderConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/identity-provider-configs/associate");
  return AssociateIdentityProviderConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateIdentityProviderConfigOutcomeCallable EKSClient::AssociateIdentityProviderConfigCallable(const AssociateIdentityProviderConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateIdentityProviderConfig, request, m_executor.get());
}

void EKSClient::AssociateIdentityProviderConfigAsync(const AssociateIdentityProviderConfigRequest& request, const AssociateIdentityProviderConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateIdentityProviderConfig, request, handler, context, m_executor.get());
}

CreateAddonOutcome EKSClient::CreateAddon(const CreateAddonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateAddon", "Required field: ClusterName, is not set");
    return CreateAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons");
  return CreateAddonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAddonOutcomeCallable EKSClient::CreateAddonCallable(const CreateAddonRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateAddon, request, m_executor.get());
}

void EKSClient::CreateAddonAsync(const CreateAddonRequest& request, const CreateAddonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateAddon, request, handler, context, m_executor.get());
}

CreateClusterOutcome EKSClient::CreateCluster(const CreateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters");
  return CreateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateClusterOutcomeCallable EKSClient::CreateClusterCallable(const CreateClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateCluster, request, m_executor.get());
}

void EKSClient::CreateClusterAsync(const CreateClusterRequest& request, const CreateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateCluster, request, handler, context, m_executor.get());
}

CreateFargateProfileOutcome EKSClient::CreateFargateProfile(const CreateFargateProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateFargateProfile", "Required field: ClusterName, is not set");
    return CreateFargateProfileOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/fargate-profiles");
  return CreateFargateProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateFargateProfileOutcomeCallable EKSClient::CreateFargateProfileCallable(const CreateFargateProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateFargateProfile, request, m_executor.get());
}

void EKSClient::CreateFargateProfileAsync(const CreateFargateProfileRequest& request, const CreateFargateProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateFargateProfile, request, handler, context, m_executor.get());
}

CreateNodegroupOutcome EKSClient::CreateNodegroup(const CreateNodegroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateNodegroup", "Required field: ClusterName, is not set");
    return CreateNodegroupOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups");
  return CreateNodegroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateNodegroupOutcomeCallable EKSClient::CreateNodegroupCallable(const CreateNodegroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateNodegroup, request, m_executor.get());
}

void EKSClient::CreateNodegroupAsync(const CreateNodegroupRequest& request, const CreateNodegroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateNodegroup, request, handler, context, m_executor.get());
}

DeleteAddonOutcome EKSClient::DeleteAddon(const DeleteAddonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAddon", "Required field: ClusterName, is not set");
    return DeleteAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.AddonNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAddon", "Required field: AddonName, is not set");
    return DeleteAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AddonName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAddonName());
  return DeleteAddonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAddonOutcomeCallable EKSClient::DeleteAddonCallable(const DeleteAddonRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAddon, request, m_executor.get());
}

void EKSClient::DeleteAddonAsync(const DeleteAddonRequest& request, const DeleteAddonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAddon, request, handler, context, m_executor.get());
}

DeleteClusterOutcome EKSClient::DeleteCluster(const DeleteClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCluster", "Required field: Name, is not set");
    return DeleteClusterOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteClusterOutcomeCallable EKSClient::DeleteClusterCallable(const DeleteClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteCluster, request, m_executor.get());
}

void EKSClient::DeleteClusterAsync(const DeleteClusterRequest& request, const DeleteClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteCluster, request, handler, context, m_executor.get());
}

DeleteFargateProfileOutcome EKSClient::DeleteFargateProfile(const DeleteFargateProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFargateProfile", "Required field: ClusterName, is not set");
    return DeleteFargateProfileOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.FargateProfileNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFargateProfile", "Required field: FargateProfileName, is not set");
    return DeleteFargateProfileOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FargateProfileName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/fargate-profiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFargateProfileName());
  return DeleteFargateProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteFargateProfileOutcomeCallable EKSClient::DeleteFargateProfileCallable(const DeleteFargateProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteFargateProfile, request, m_executor.get());
}

void EKSClient::DeleteFargateProfileAsync(const DeleteFargateProfileRequest& request, const DeleteFargateProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteFargateProfile, request, handler, context, m_executor.get());
}

DeleteNodegroupOutcome EKSClient::DeleteNodegroup(const DeleteNodegroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteNodegroup", "Required field: ClusterName, is not set");
    return DeleteNodegroupOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.NodegroupNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteNodegroup", "Required field: NodegroupName, is not set");
    return DeleteNodegroupOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [NodegroupName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNodegroupName());
  return DeleteNodegroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteNodegroupOutcomeCallable EKSClient::DeleteNodegroupCallable(const DeleteNodegroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteNodegroup, request, m_executor.get());
}

void EKSClient::DeleteNodegroupAsync(const DeleteNodegroupRequest& request, const DeleteNodegroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteNodegroup, request, handler, context, m_executor.get());
}

DeregisterClusterOutcome EKSClient::DeregisterCluster(const DeregisterClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeregisterCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeregisterCluster", "Required field: Name, is not set");
    return DeregisterClusterOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeregisterCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster-registrations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeregisterClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeregisterClusterOutcomeCallable EKSClient::DeregisterClusterCallable(const DeregisterClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeregisterCluster, request, m_executor.get());
}

void EKSClient::DeregisterClusterAsync(const DeregisterClusterRequest& request, const DeregisterClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeregisterCluster, request, handler, context, m_executor.get());
}

DescribeAddonOutcome EKSClient::DescribeAddon(const DescribeAddonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAddon", "Required field: ClusterName, is not set");
    return DescribeAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.AddonNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeAddon", "Required field: AddonName, is not set");
    return DescribeAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AddonName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAddonName());
  return DescribeAddonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAddonOutcomeCallable EKSClient::DescribeAddonCallable(const DescribeAddonRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAddon, request, m_executor.get());
}

void EKSClient::DescribeAddonAsync(const DescribeAddonRequest& request, const DescribeAddonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAddon, request, handler, context, m_executor.get());
}

DescribeAddonVersionsOutcome EKSClient::DescribeAddonVersions(const DescribeAddonVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAddonVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAddonVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons/supported-versions");
  return DescribeAddonVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeAddonVersionsOutcomeCallable EKSClient::DescribeAddonVersionsCallable(const DescribeAddonVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeAddonVersions, request, m_executor.get());
}

void EKSClient::DescribeAddonVersionsAsync(const DescribeAddonVersionsRequest& request, const DescribeAddonVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeAddonVersions, request, handler, context, m_executor.get());
}

DescribeClusterOutcome EKSClient::DescribeCluster(const DescribeClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeCluster", "Required field: Name, is not set");
    return DescribeClusterOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DescribeClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeClusterOutcomeCallable EKSClient::DescribeClusterCallable(const DescribeClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeCluster, request, m_executor.get());
}

void EKSClient::DescribeClusterAsync(const DescribeClusterRequest& request, const DescribeClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeCluster, request, handler, context, m_executor.get());
}

DescribeFargateProfileOutcome EKSClient::DescribeFargateProfile(const DescribeFargateProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeFargateProfile", "Required field: ClusterName, is not set");
    return DescribeFargateProfileOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.FargateProfileNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeFargateProfile", "Required field: FargateProfileName, is not set");
    return DescribeFargateProfileOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FargateProfileName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeFargateProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/fargate-profiles/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFargateProfileName());
  return DescribeFargateProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeFargateProfileOutcomeCallable EKSClient::DescribeFargateProfileCallable(const DescribeFargateProfileRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeFargateProfile, request, m_executor.get());
}

void EKSClient::DescribeFargateProfileAsync(const DescribeFargateProfileRequest& request, const DescribeFargateProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeFargateProfile, request, handler, context, m_executor.get());
}

DescribeIdentityProviderConfigOutcome EKSClient::DescribeIdentityProviderConfig(const DescribeIdentityProviderConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeIdentityProviderConfig", "Required field: ClusterName, is not set");
    return DescribeIdentityProviderConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/identity-provider-configs/describe");
  return DescribeIdentityProviderConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeIdentityProviderConfigOutcomeCallable EKSClient::DescribeIdentityProviderConfigCallable(const DescribeIdentityProviderConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeIdentityProviderConfig, request, m_executor.get());
}

void EKSClient::DescribeIdentityProviderConfigAsync(const DescribeIdentityProviderConfigRequest& request, const DescribeIdentityProviderConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeIdentityProviderConfig, request, handler, context, m_executor.get());
}

DescribeNodegroupOutcome EKSClient::DescribeNodegroup(const DescribeNodegroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeNodegroup", "Required field: ClusterName, is not set");
    return DescribeNodegroupOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.NodegroupNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeNodegroup", "Required field: NodegroupName, is not set");
    return DescribeNodegroupOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [NodegroupName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeNodegroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNodegroupName());
  return DescribeNodegroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeNodegroupOutcomeCallable EKSClient::DescribeNodegroupCallable(const DescribeNodegroupRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeNodegroup, request, m_executor.get());
}

void EKSClient::DescribeNodegroupAsync(const DescribeNodegroupRequest& request, const DescribeNodegroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeNodegroup, request, handler, context, m_executor.get());
}

DescribeUpdateOutcome EKSClient::DescribeUpdate(const DescribeUpdateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUpdate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeUpdate", "Required field: Name, is not set");
    return DescribeUpdateOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  if (!request.UpdateIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeUpdate", "Required field: UpdateId, is not set");
    return DescribeUpdateOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [UpdateId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUpdate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updates/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetUpdateId());
  return DescribeUpdateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeUpdateOutcomeCallable EKSClient::DescribeUpdateCallable(const DescribeUpdateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeUpdate, request, m_executor.get());
}

void EKSClient::DescribeUpdateAsync(const DescribeUpdateRequest& request, const DescribeUpdateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeUpdate, request, handler, context, m_executor.get());
}

DisassociateIdentityProviderConfigOutcome EKSClient::DisassociateIdentityProviderConfig(const DisassociateIdentityProviderConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateIdentityProviderConfig", "Required field: ClusterName, is not set");
    return DisassociateIdentityProviderConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateIdentityProviderConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/identity-provider-configs/disassociate");
  return DisassociateIdentityProviderConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateIdentityProviderConfigOutcomeCallable EKSClient::DisassociateIdentityProviderConfigCallable(const DisassociateIdentityProviderConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateIdentityProviderConfig, request, m_executor.get());
}

void EKSClient::DisassociateIdentityProviderConfigAsync(const DisassociateIdentityProviderConfigRequest& request, const DisassociateIdentityProviderConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateIdentityProviderConfig, request, handler, context, m_executor.get());
}

ListAddonsOutcome EKSClient::ListAddons(const ListAddonsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAddons, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListAddons", "Required field: ClusterName, is not set");
    return ListAddonsOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAddons, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons");
  return ListAddonsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAddonsOutcomeCallable EKSClient::ListAddonsCallable(const ListAddonsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAddons, request, m_executor.get());
}

void EKSClient::ListAddonsAsync(const ListAddonsRequest& request, const ListAddonsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAddons, request, handler, context, m_executor.get());
}

ListClustersOutcome EKSClient::ListClusters(const ListClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters");
  return ListClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListClustersOutcomeCallable EKSClient::ListClustersCallable(const ListClustersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListClusters, request, m_executor.get());
}

void EKSClient::ListClustersAsync(const ListClustersRequest& request, const ListClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListClusters, request, handler, context, m_executor.get());
}

ListFargateProfilesOutcome EKSClient::ListFargateProfiles(const ListFargateProfilesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListFargateProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListFargateProfiles", "Required field: ClusterName, is not set");
    return ListFargateProfilesOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListFargateProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/fargate-profiles");
  return ListFargateProfilesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListFargateProfilesOutcomeCallable EKSClient::ListFargateProfilesCallable(const ListFargateProfilesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListFargateProfiles, request, m_executor.get());
}

void EKSClient::ListFargateProfilesAsync(const ListFargateProfilesRequest& request, const ListFargateProfilesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListFargateProfiles, request, handler, context, m_executor.get());
}

ListIdentityProviderConfigsOutcome EKSClient::ListIdentityProviderConfigs(const ListIdentityProviderConfigsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIdentityProviderConfigs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListIdentityProviderConfigs", "Required field: ClusterName, is not set");
    return ListIdentityProviderConfigsOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIdentityProviderConfigs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/identity-provider-configs");
  return ListIdentityProviderConfigsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListIdentityProviderConfigsOutcomeCallable EKSClient::ListIdentityProviderConfigsCallable(const ListIdentityProviderConfigsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListIdentityProviderConfigs, request, m_executor.get());
}

void EKSClient::ListIdentityProviderConfigsAsync(const ListIdentityProviderConfigsRequest& request, const ListIdentityProviderConfigsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListIdentityProviderConfigs, request, handler, context, m_executor.get());
}

ListNodegroupsOutcome EKSClient::ListNodegroups(const ListNodegroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListNodegroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListNodegroups", "Required field: ClusterName, is not set");
    return ListNodegroupsOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListNodegroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups");
  return ListNodegroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListNodegroupsOutcomeCallable EKSClient::ListNodegroupsCallable(const ListNodegroupsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListNodegroups, request, m_executor.get());
}

void EKSClient::ListNodegroupsAsync(const ListNodegroupsRequest& request, const ListNodegroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListNodegroups, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome EKSClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable EKSClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void EKSClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

ListUpdatesOutcome EKSClient::ListUpdates(const ListUpdatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListUpdates", "Required field: Name, is not set");
    return ListUpdatesOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUpdates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updates");
  return ListUpdatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListUpdatesOutcomeCallable EKSClient::ListUpdatesCallable(const ListUpdatesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListUpdates, request, m_executor.get());
}

void EKSClient::ListUpdatesAsync(const ListUpdatesRequest& request, const ListUpdatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListUpdates, request, handler, context, m_executor.get());
}

RegisterClusterOutcome EKSClient::RegisterCluster(const RegisterClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cluster-registrations");
  return RegisterClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RegisterClusterOutcomeCallable EKSClient::RegisterClusterCallable(const RegisterClusterRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(RegisterCluster, request, m_executor.get());
}

void EKSClient::RegisterClusterAsync(const RegisterClusterRequest& request, const RegisterClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(RegisterCluster, request, handler, context, m_executor.get());
}

TagResourceOutcome EKSClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable EKSClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void EKSClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome EKSClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable EKSClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void EKSClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateAddonOutcome EKSClient::UpdateAddon(const UpdateAddonRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAddon", "Required field: ClusterName, is not set");
    return UpdateAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.AddonNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateAddon", "Required field: AddonName, is not set");
    return UpdateAddonOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AddonName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAddon, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/addons/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAddonName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update");
  return UpdateAddonOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateAddonOutcomeCallable EKSClient::UpdateAddonCallable(const UpdateAddonRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateAddon, request, m_executor.get());
}

void EKSClient::UpdateAddonAsync(const UpdateAddonRequest& request, const UpdateAddonResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateAddon, request, handler, context, m_executor.get());
}

UpdateClusterConfigOutcome EKSClient::UpdateClusterConfig(const UpdateClusterConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateClusterConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateClusterConfig", "Required field: Name, is not set");
    return UpdateClusterConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateClusterConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-config");
  return UpdateClusterConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateClusterConfigOutcomeCallable EKSClient::UpdateClusterConfigCallable(const UpdateClusterConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateClusterConfig, request, m_executor.get());
}

void EKSClient::UpdateClusterConfigAsync(const UpdateClusterConfigRequest& request, const UpdateClusterConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateClusterConfig, request, handler, context, m_executor.get());
}

UpdateClusterVersionOutcome EKSClient::UpdateClusterVersion(const UpdateClusterVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateClusterVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateClusterVersion", "Required field: Name, is not set");
    return UpdateClusterVersionOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateClusterVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updates");
  return UpdateClusterVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateClusterVersionOutcomeCallable EKSClient::UpdateClusterVersionCallable(const UpdateClusterVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateClusterVersion, request, m_executor.get());
}

void EKSClient::UpdateClusterVersionAsync(const UpdateClusterVersionRequest& request, const UpdateClusterVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateClusterVersion, request, handler, context, m_executor.get());
}

UpdateNodegroupConfigOutcome EKSClient::UpdateNodegroupConfig(const UpdateNodegroupConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateNodegroupConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNodegroupConfig", "Required field: ClusterName, is not set");
    return UpdateNodegroupConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.NodegroupNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNodegroupConfig", "Required field: NodegroupName, is not set");
    return UpdateNodegroupConfigOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [NodegroupName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateNodegroupConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNodegroupName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-config");
  return UpdateNodegroupConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateNodegroupConfigOutcomeCallable EKSClient::UpdateNodegroupConfigCallable(const UpdateNodegroupConfigRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateNodegroupConfig, request, m_executor.get());
}

void EKSClient::UpdateNodegroupConfigAsync(const UpdateNodegroupConfigRequest& request, const UpdateNodegroupConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateNodegroupConfig, request, handler, context, m_executor.get());
}

UpdateNodegroupVersionOutcome EKSClient::UpdateNodegroupVersion(const UpdateNodegroupVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateNodegroupVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ClusterNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNodegroupVersion", "Required field: ClusterName, is not set");
    return UpdateNodegroupVersionOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ClusterName]", false));
  }
  if (!request.NodegroupNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNodegroupVersion", "Required field: NodegroupName, is not set");
    return UpdateNodegroupVersionOutcome(Aws::Client::AWSError<EKSErrors>(EKSErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [NodegroupName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateNodegroupVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/clusters/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetClusterName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/node-groups/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetNodegroupName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/update-version");
  return UpdateNodegroupVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateNodegroupVersionOutcomeCallable EKSClient::UpdateNodegroupVersionCallable(const UpdateNodegroupVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateNodegroupVersion, request, m_executor.get());
}

void EKSClient::UpdateNodegroupVersionAsync(const UpdateNodegroupVersionRequest& request, const UpdateNodegroupVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateNodegroupVersion, request, handler, context, m_executor.get());
}

