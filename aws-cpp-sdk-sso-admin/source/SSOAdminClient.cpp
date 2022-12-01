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

#include <aws/sso-admin/SSOAdminClient.h>
#include <aws/sso-admin/SSOAdminErrorMarshaller.h>
#include <aws/sso-admin/SSOAdminEndpointProvider.h>
#include <aws/sso-admin/model/AttachCustomerManagedPolicyReferenceToPermissionSetRequest.h>
#include <aws/sso-admin/model/AttachManagedPolicyToPermissionSetRequest.h>
#include <aws/sso-admin/model/CreateAccountAssignmentRequest.h>
#include <aws/sso-admin/model/CreateInstanceAccessControlAttributeConfigurationRequest.h>
#include <aws/sso-admin/model/CreatePermissionSetRequest.h>
#include <aws/sso-admin/model/DeleteAccountAssignmentRequest.h>
#include <aws/sso-admin/model/DeleteInlinePolicyFromPermissionSetRequest.h>
#include <aws/sso-admin/model/DeleteInstanceAccessControlAttributeConfigurationRequest.h>
#include <aws/sso-admin/model/DeletePermissionSetRequest.h>
#include <aws/sso-admin/model/DeletePermissionsBoundaryFromPermissionSetRequest.h>
#include <aws/sso-admin/model/DescribeAccountAssignmentCreationStatusRequest.h>
#include <aws/sso-admin/model/DescribeAccountAssignmentDeletionStatusRequest.h>
#include <aws/sso-admin/model/DescribeInstanceAccessControlAttributeConfigurationRequest.h>
#include <aws/sso-admin/model/DescribePermissionSetRequest.h>
#include <aws/sso-admin/model/DescribePermissionSetProvisioningStatusRequest.h>
#include <aws/sso-admin/model/DetachCustomerManagedPolicyReferenceFromPermissionSetRequest.h>
#include <aws/sso-admin/model/DetachManagedPolicyFromPermissionSetRequest.h>
#include <aws/sso-admin/model/GetInlinePolicyForPermissionSetRequest.h>
#include <aws/sso-admin/model/GetPermissionsBoundaryForPermissionSetRequest.h>
#include <aws/sso-admin/model/ListAccountAssignmentCreationStatusRequest.h>
#include <aws/sso-admin/model/ListAccountAssignmentDeletionStatusRequest.h>
#include <aws/sso-admin/model/ListAccountAssignmentsRequest.h>
#include <aws/sso-admin/model/ListAccountsForProvisionedPermissionSetRequest.h>
#include <aws/sso-admin/model/ListCustomerManagedPolicyReferencesInPermissionSetRequest.h>
#include <aws/sso-admin/model/ListInstancesRequest.h>
#include <aws/sso-admin/model/ListManagedPoliciesInPermissionSetRequest.h>
#include <aws/sso-admin/model/ListPermissionSetProvisioningStatusRequest.h>
#include <aws/sso-admin/model/ListPermissionSetsRequest.h>
#include <aws/sso-admin/model/ListPermissionSetsProvisionedToAccountRequest.h>
#include <aws/sso-admin/model/ListTagsForResourceRequest.h>
#include <aws/sso-admin/model/ProvisionPermissionSetRequest.h>
#include <aws/sso-admin/model/PutInlinePolicyToPermissionSetRequest.h>
#include <aws/sso-admin/model/PutPermissionsBoundaryToPermissionSetRequest.h>
#include <aws/sso-admin/model/TagResourceRequest.h>
#include <aws/sso-admin/model/UntagResourceRequest.h>
#include <aws/sso-admin/model/UpdateInstanceAccessControlAttributeConfigurationRequest.h>
#include <aws/sso-admin/model/UpdatePermissionSetRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::SSOAdmin;
using namespace Aws::SSOAdmin::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* SSOAdminClient::SERVICE_NAME = "sso";
const char* SSOAdminClient::ALLOCATION_TAG = "SSOAdminClient";

SSOAdminClient::SSOAdminClient(const SSOAdmin::SSOAdminClientConfiguration& clientConfiguration,
                               std::shared_ptr<SSOAdminEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

SSOAdminClient::SSOAdminClient(const AWSCredentials& credentials,
                               std::shared_ptr<SSOAdminEndpointProviderBase> endpointProvider,
                               const SSOAdmin::SSOAdminClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

SSOAdminClient::SSOAdminClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<SSOAdminEndpointProviderBase> endpointProvider,
                               const SSOAdmin::SSOAdminClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  SSOAdminClient::SSOAdminClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<SSOAdminEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

SSOAdminClient::SSOAdminClient(const AWSCredentials& credentials,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<SSOAdminEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

SSOAdminClient::SSOAdminClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<SSOAdminErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<SSOAdminEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
SSOAdminClient::~SSOAdminClient()
{
}

std::shared_ptr<SSOAdminEndpointProviderBase>& SSOAdminClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void SSOAdminClient::init(const SSOAdmin::SSOAdminClientConfiguration& config)
{
  AWSClient::SetServiceClientName("SSO Admin");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void SSOAdminClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AttachCustomerManagedPolicyReferenceToPermissionSetOutcome SSOAdminClient::AttachCustomerManagedPolicyReferenceToPermissionSet(const AttachCustomerManagedPolicyReferenceToPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachCustomerManagedPolicyReferenceToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachCustomerManagedPolicyReferenceToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AttachCustomerManagedPolicyReferenceToPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AttachCustomerManagedPolicyReferenceToPermissionSetOutcomeCallable SSOAdminClient::AttachCustomerManagedPolicyReferenceToPermissionSetCallable(const AttachCustomerManagedPolicyReferenceToPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::AttachCustomerManagedPolicyReferenceToPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::AttachCustomerManagedPolicyReferenceToPermissionSetAsync(const AttachCustomerManagedPolicyReferenceToPermissionSetRequest& request, const AttachCustomerManagedPolicyReferenceToPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::AttachCustomerManagedPolicyReferenceToPermissionSet, this, request, handler, context, m_executor.get());
}

AttachManagedPolicyToPermissionSetOutcome SSOAdminClient::AttachManagedPolicyToPermissionSet(const AttachManagedPolicyToPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AttachManagedPolicyToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AttachManagedPolicyToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AttachManagedPolicyToPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AttachManagedPolicyToPermissionSetOutcomeCallable SSOAdminClient::AttachManagedPolicyToPermissionSetCallable(const AttachManagedPolicyToPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::AttachManagedPolicyToPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::AttachManagedPolicyToPermissionSetAsync(const AttachManagedPolicyToPermissionSetRequest& request, const AttachManagedPolicyToPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::AttachManagedPolicyToPermissionSet, this, request, handler, context, m_executor.get());
}

CreateAccountAssignmentOutcome SSOAdminClient::CreateAccountAssignment(const CreateAccountAssignmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAccountAssignment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAccountAssignment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateAccountAssignmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateAccountAssignmentOutcomeCallable SSOAdminClient::CreateAccountAssignmentCallable(const CreateAccountAssignmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::CreateAccountAssignment, this, request, m_executor.get());
}

void SSOAdminClient::CreateAccountAssignmentAsync(const CreateAccountAssignmentRequest& request, const CreateAccountAssignmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::CreateAccountAssignment, this, request, handler, context, m_executor.get());
}

CreateInstanceAccessControlAttributeConfigurationOutcome SSOAdminClient::CreateInstanceAccessControlAttributeConfiguration(const CreateInstanceAccessControlAttributeConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateInstanceAccessControlAttributeConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateInstanceAccessControlAttributeConfigurationOutcomeCallable SSOAdminClient::CreateInstanceAccessControlAttributeConfigurationCallable(const CreateInstanceAccessControlAttributeConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::CreateInstanceAccessControlAttributeConfiguration, this, request, m_executor.get());
}

void SSOAdminClient::CreateInstanceAccessControlAttributeConfigurationAsync(const CreateInstanceAccessControlAttributeConfigurationRequest& request, const CreateInstanceAccessControlAttributeConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::CreateInstanceAccessControlAttributeConfiguration, this, request, handler, context, m_executor.get());
}

CreatePermissionSetOutcome SSOAdminClient::CreatePermissionSet(const CreatePermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreatePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreatePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreatePermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreatePermissionSetOutcomeCallable SSOAdminClient::CreatePermissionSetCallable(const CreatePermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::CreatePermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::CreatePermissionSetAsync(const CreatePermissionSetRequest& request, const CreatePermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::CreatePermissionSet, this, request, handler, context, m_executor.get());
}

DeleteAccountAssignmentOutcome SSOAdminClient::DeleteAccountAssignment(const DeleteAccountAssignmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAccountAssignment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAccountAssignment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteAccountAssignmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteAccountAssignmentOutcomeCallable SSOAdminClient::DeleteAccountAssignmentCallable(const DeleteAccountAssignmentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DeleteAccountAssignment, this, request, m_executor.get());
}

void SSOAdminClient::DeleteAccountAssignmentAsync(const DeleteAccountAssignmentRequest& request, const DeleteAccountAssignmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DeleteAccountAssignment, this, request, handler, context, m_executor.get());
}

DeleteInlinePolicyFromPermissionSetOutcome SSOAdminClient::DeleteInlinePolicyFromPermissionSet(const DeleteInlinePolicyFromPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteInlinePolicyFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteInlinePolicyFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteInlinePolicyFromPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteInlinePolicyFromPermissionSetOutcomeCallable SSOAdminClient::DeleteInlinePolicyFromPermissionSetCallable(const DeleteInlinePolicyFromPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DeleteInlinePolicyFromPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DeleteInlinePolicyFromPermissionSetAsync(const DeleteInlinePolicyFromPermissionSetRequest& request, const DeleteInlinePolicyFromPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DeleteInlinePolicyFromPermissionSet, this, request, handler, context, m_executor.get());
}

DeleteInstanceAccessControlAttributeConfigurationOutcome SSOAdminClient::DeleteInstanceAccessControlAttributeConfiguration(const DeleteInstanceAccessControlAttributeConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteInstanceAccessControlAttributeConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteInstanceAccessControlAttributeConfigurationOutcomeCallable SSOAdminClient::DeleteInstanceAccessControlAttributeConfigurationCallable(const DeleteInstanceAccessControlAttributeConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DeleteInstanceAccessControlAttributeConfiguration, this, request, m_executor.get());
}

void SSOAdminClient::DeleteInstanceAccessControlAttributeConfigurationAsync(const DeleteInstanceAccessControlAttributeConfigurationRequest& request, const DeleteInstanceAccessControlAttributeConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DeleteInstanceAccessControlAttributeConfiguration, this, request, handler, context, m_executor.get());
}

DeletePermissionSetOutcome SSOAdminClient::DeletePermissionSet(const DeletePermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePermissionSetOutcomeCallable SSOAdminClient::DeletePermissionSetCallable(const DeletePermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DeletePermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DeletePermissionSetAsync(const DeletePermissionSetRequest& request, const DeletePermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DeletePermissionSet, this, request, handler, context, m_executor.get());
}

DeletePermissionsBoundaryFromPermissionSetOutcome SSOAdminClient::DeletePermissionsBoundaryFromPermissionSet(const DeletePermissionsBoundaryFromPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePermissionsBoundaryFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePermissionsBoundaryFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePermissionsBoundaryFromPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeletePermissionsBoundaryFromPermissionSetOutcomeCallable SSOAdminClient::DeletePermissionsBoundaryFromPermissionSetCallable(const DeletePermissionsBoundaryFromPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DeletePermissionsBoundaryFromPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DeletePermissionsBoundaryFromPermissionSetAsync(const DeletePermissionsBoundaryFromPermissionSetRequest& request, const DeletePermissionsBoundaryFromPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DeletePermissionsBoundaryFromPermissionSet, this, request, handler, context, m_executor.get());
}

DescribeAccountAssignmentCreationStatusOutcome SSOAdminClient::DescribeAccountAssignmentCreationStatus(const DescribeAccountAssignmentCreationStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountAssignmentCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountAssignmentCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountAssignmentCreationStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccountAssignmentCreationStatusOutcomeCallable SSOAdminClient::DescribeAccountAssignmentCreationStatusCallable(const DescribeAccountAssignmentCreationStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DescribeAccountAssignmentCreationStatus, this, request, m_executor.get());
}

void SSOAdminClient::DescribeAccountAssignmentCreationStatusAsync(const DescribeAccountAssignmentCreationStatusRequest& request, const DescribeAccountAssignmentCreationStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DescribeAccountAssignmentCreationStatus, this, request, handler, context, m_executor.get());
}

DescribeAccountAssignmentDeletionStatusOutcome SSOAdminClient::DescribeAccountAssignmentDeletionStatus(const DescribeAccountAssignmentDeletionStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountAssignmentDeletionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountAssignmentDeletionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountAssignmentDeletionStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccountAssignmentDeletionStatusOutcomeCallable SSOAdminClient::DescribeAccountAssignmentDeletionStatusCallable(const DescribeAccountAssignmentDeletionStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DescribeAccountAssignmentDeletionStatus, this, request, m_executor.get());
}

void SSOAdminClient::DescribeAccountAssignmentDeletionStatusAsync(const DescribeAccountAssignmentDeletionStatusRequest& request, const DescribeAccountAssignmentDeletionStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DescribeAccountAssignmentDeletionStatus, this, request, handler, context, m_executor.get());
}

DescribeInstanceAccessControlAttributeConfigurationOutcome SSOAdminClient::DescribeInstanceAccessControlAttributeConfiguration(const DescribeInstanceAccessControlAttributeConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeInstanceAccessControlAttributeConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeInstanceAccessControlAttributeConfigurationOutcomeCallable SSOAdminClient::DescribeInstanceAccessControlAttributeConfigurationCallable(const DescribeInstanceAccessControlAttributeConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DescribeInstanceAccessControlAttributeConfiguration, this, request, m_executor.get());
}

void SSOAdminClient::DescribeInstanceAccessControlAttributeConfigurationAsync(const DescribeInstanceAccessControlAttributeConfigurationRequest& request, const DescribeInstanceAccessControlAttributeConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DescribeInstanceAccessControlAttributeConfiguration, this, request, handler, context, m_executor.get());
}

DescribePermissionSetOutcome SSOAdminClient::DescribePermissionSet(const DescribePermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribePermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribePermissionSetOutcomeCallable SSOAdminClient::DescribePermissionSetCallable(const DescribePermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DescribePermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DescribePermissionSetAsync(const DescribePermissionSetRequest& request, const DescribePermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DescribePermissionSet, this, request, handler, context, m_executor.get());
}

DescribePermissionSetProvisioningStatusOutcome SSOAdminClient::DescribePermissionSetProvisioningStatus(const DescribePermissionSetProvisioningStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribePermissionSetProvisioningStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribePermissionSetProvisioningStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribePermissionSetProvisioningStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribePermissionSetProvisioningStatusOutcomeCallable SSOAdminClient::DescribePermissionSetProvisioningStatusCallable(const DescribePermissionSetProvisioningStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DescribePermissionSetProvisioningStatus, this, request, m_executor.get());
}

void SSOAdminClient::DescribePermissionSetProvisioningStatusAsync(const DescribePermissionSetProvisioningStatusRequest& request, const DescribePermissionSetProvisioningStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DescribePermissionSetProvisioningStatus, this, request, handler, context, m_executor.get());
}

DetachCustomerManagedPolicyReferenceFromPermissionSetOutcome SSOAdminClient::DetachCustomerManagedPolicyReferenceFromPermissionSet(const DetachCustomerManagedPolicyReferenceFromPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachCustomerManagedPolicyReferenceFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachCustomerManagedPolicyReferenceFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DetachCustomerManagedPolicyReferenceFromPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DetachCustomerManagedPolicyReferenceFromPermissionSetOutcomeCallable SSOAdminClient::DetachCustomerManagedPolicyReferenceFromPermissionSetCallable(const DetachCustomerManagedPolicyReferenceFromPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DetachCustomerManagedPolicyReferenceFromPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DetachCustomerManagedPolicyReferenceFromPermissionSetAsync(const DetachCustomerManagedPolicyReferenceFromPermissionSetRequest& request, const DetachCustomerManagedPolicyReferenceFromPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DetachCustomerManagedPolicyReferenceFromPermissionSet, this, request, handler, context, m_executor.get());
}

DetachManagedPolicyFromPermissionSetOutcome SSOAdminClient::DetachManagedPolicyFromPermissionSet(const DetachManagedPolicyFromPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetachManagedPolicyFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetachManagedPolicyFromPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DetachManagedPolicyFromPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DetachManagedPolicyFromPermissionSetOutcomeCallable SSOAdminClient::DetachManagedPolicyFromPermissionSetCallable(const DetachManagedPolicyFromPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::DetachManagedPolicyFromPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::DetachManagedPolicyFromPermissionSetAsync(const DetachManagedPolicyFromPermissionSetRequest& request, const DetachManagedPolicyFromPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::DetachManagedPolicyFromPermissionSet, this, request, handler, context, m_executor.get());
}

GetInlinePolicyForPermissionSetOutcome SSOAdminClient::GetInlinePolicyForPermissionSet(const GetInlinePolicyForPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetInlinePolicyForPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetInlinePolicyForPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetInlinePolicyForPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetInlinePolicyForPermissionSetOutcomeCallable SSOAdminClient::GetInlinePolicyForPermissionSetCallable(const GetInlinePolicyForPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::GetInlinePolicyForPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::GetInlinePolicyForPermissionSetAsync(const GetInlinePolicyForPermissionSetRequest& request, const GetInlinePolicyForPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::GetInlinePolicyForPermissionSet, this, request, handler, context, m_executor.get());
}

GetPermissionsBoundaryForPermissionSetOutcome SSOAdminClient::GetPermissionsBoundaryForPermissionSet(const GetPermissionsBoundaryForPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetPermissionsBoundaryForPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetPermissionsBoundaryForPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetPermissionsBoundaryForPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetPermissionsBoundaryForPermissionSetOutcomeCallable SSOAdminClient::GetPermissionsBoundaryForPermissionSetCallable(const GetPermissionsBoundaryForPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::GetPermissionsBoundaryForPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::GetPermissionsBoundaryForPermissionSetAsync(const GetPermissionsBoundaryForPermissionSetRequest& request, const GetPermissionsBoundaryForPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::GetPermissionsBoundaryForPermissionSet, this, request, handler, context, m_executor.get());
}

ListAccountAssignmentCreationStatusOutcome SSOAdminClient::ListAccountAssignmentCreationStatus(const ListAccountAssignmentCreationStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAccountAssignmentCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAccountAssignmentCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAccountAssignmentCreationStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAccountAssignmentCreationStatusOutcomeCallable SSOAdminClient::ListAccountAssignmentCreationStatusCallable(const ListAccountAssignmentCreationStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListAccountAssignmentCreationStatus, this, request, m_executor.get());
}

void SSOAdminClient::ListAccountAssignmentCreationStatusAsync(const ListAccountAssignmentCreationStatusRequest& request, const ListAccountAssignmentCreationStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListAccountAssignmentCreationStatus, this, request, handler, context, m_executor.get());
}

ListAccountAssignmentDeletionStatusOutcome SSOAdminClient::ListAccountAssignmentDeletionStatus(const ListAccountAssignmentDeletionStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAccountAssignmentDeletionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAccountAssignmentDeletionStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAccountAssignmentDeletionStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAccountAssignmentDeletionStatusOutcomeCallable SSOAdminClient::ListAccountAssignmentDeletionStatusCallable(const ListAccountAssignmentDeletionStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListAccountAssignmentDeletionStatus, this, request, m_executor.get());
}

void SSOAdminClient::ListAccountAssignmentDeletionStatusAsync(const ListAccountAssignmentDeletionStatusRequest& request, const ListAccountAssignmentDeletionStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListAccountAssignmentDeletionStatus, this, request, handler, context, m_executor.get());
}

ListAccountAssignmentsOutcome SSOAdminClient::ListAccountAssignments(const ListAccountAssignmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAccountAssignments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAccountAssignments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAccountAssignmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAccountAssignmentsOutcomeCallable SSOAdminClient::ListAccountAssignmentsCallable(const ListAccountAssignmentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListAccountAssignments, this, request, m_executor.get());
}

void SSOAdminClient::ListAccountAssignmentsAsync(const ListAccountAssignmentsRequest& request, const ListAccountAssignmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListAccountAssignments, this, request, handler, context, m_executor.get());
}

ListAccountsForProvisionedPermissionSetOutcome SSOAdminClient::ListAccountsForProvisionedPermissionSet(const ListAccountsForProvisionedPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAccountsForProvisionedPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAccountsForProvisionedPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListAccountsForProvisionedPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListAccountsForProvisionedPermissionSetOutcomeCallable SSOAdminClient::ListAccountsForProvisionedPermissionSetCallable(const ListAccountsForProvisionedPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListAccountsForProvisionedPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::ListAccountsForProvisionedPermissionSetAsync(const ListAccountsForProvisionedPermissionSetRequest& request, const ListAccountsForProvisionedPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListAccountsForProvisionedPermissionSet, this, request, handler, context, m_executor.get());
}

ListCustomerManagedPolicyReferencesInPermissionSetOutcome SSOAdminClient::ListCustomerManagedPolicyReferencesInPermissionSet(const ListCustomerManagedPolicyReferencesInPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCustomerManagedPolicyReferencesInPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCustomerManagedPolicyReferencesInPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListCustomerManagedPolicyReferencesInPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListCustomerManagedPolicyReferencesInPermissionSetOutcomeCallable SSOAdminClient::ListCustomerManagedPolicyReferencesInPermissionSetCallable(const ListCustomerManagedPolicyReferencesInPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListCustomerManagedPolicyReferencesInPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::ListCustomerManagedPolicyReferencesInPermissionSetAsync(const ListCustomerManagedPolicyReferencesInPermissionSetRequest& request, const ListCustomerManagedPolicyReferencesInPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListCustomerManagedPolicyReferencesInPermissionSet, this, request, handler, context, m_executor.get());
}

ListInstancesOutcome SSOAdminClient::ListInstances(const ListInstancesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListInstances, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListInstancesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListInstancesOutcomeCallable SSOAdminClient::ListInstancesCallable(const ListInstancesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListInstances, this, request, m_executor.get());
}

void SSOAdminClient::ListInstancesAsync(const ListInstancesRequest& request, const ListInstancesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListInstances, this, request, handler, context, m_executor.get());
}

ListManagedPoliciesInPermissionSetOutcome SSOAdminClient::ListManagedPoliciesInPermissionSet(const ListManagedPoliciesInPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListManagedPoliciesInPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListManagedPoliciesInPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListManagedPoliciesInPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListManagedPoliciesInPermissionSetOutcomeCallable SSOAdminClient::ListManagedPoliciesInPermissionSetCallable(const ListManagedPoliciesInPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListManagedPoliciesInPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::ListManagedPoliciesInPermissionSetAsync(const ListManagedPoliciesInPermissionSetRequest& request, const ListManagedPoliciesInPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListManagedPoliciesInPermissionSet, this, request, handler, context, m_executor.get());
}

ListPermissionSetProvisioningStatusOutcome SSOAdminClient::ListPermissionSetProvisioningStatus(const ListPermissionSetProvisioningStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPermissionSetProvisioningStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPermissionSetProvisioningStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListPermissionSetProvisioningStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPermissionSetProvisioningStatusOutcomeCallable SSOAdminClient::ListPermissionSetProvisioningStatusCallable(const ListPermissionSetProvisioningStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListPermissionSetProvisioningStatus, this, request, m_executor.get());
}

void SSOAdminClient::ListPermissionSetProvisioningStatusAsync(const ListPermissionSetProvisioningStatusRequest& request, const ListPermissionSetProvisioningStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListPermissionSetProvisioningStatus, this, request, handler, context, m_executor.get());
}

ListPermissionSetsOutcome SSOAdminClient::ListPermissionSets(const ListPermissionSetsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPermissionSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPermissionSets, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListPermissionSetsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPermissionSetsOutcomeCallable SSOAdminClient::ListPermissionSetsCallable(const ListPermissionSetsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListPermissionSets, this, request, m_executor.get());
}

void SSOAdminClient::ListPermissionSetsAsync(const ListPermissionSetsRequest& request, const ListPermissionSetsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListPermissionSets, this, request, handler, context, m_executor.get());
}

ListPermissionSetsProvisionedToAccountOutcome SSOAdminClient::ListPermissionSetsProvisionedToAccount(const ListPermissionSetsProvisionedToAccountRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPermissionSetsProvisionedToAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPermissionSetsProvisionedToAccount, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListPermissionSetsProvisionedToAccountOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListPermissionSetsProvisionedToAccountOutcomeCallable SSOAdminClient::ListPermissionSetsProvisionedToAccountCallable(const ListPermissionSetsProvisionedToAccountRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListPermissionSetsProvisionedToAccount, this, request, m_executor.get());
}

void SSOAdminClient::ListPermissionSetsProvisionedToAccountAsync(const ListPermissionSetsProvisionedToAccountRequest& request, const ListPermissionSetsProvisionedToAccountResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListPermissionSetsProvisionedToAccount, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome SSOAdminClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable SSOAdminClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ListTagsForResource, this, request, m_executor.get());
}

void SSOAdminClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

ProvisionPermissionSetOutcome SSOAdminClient::ProvisionPermissionSet(const ProvisionPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ProvisionPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ProvisionPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ProvisionPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ProvisionPermissionSetOutcomeCallable SSOAdminClient::ProvisionPermissionSetCallable(const ProvisionPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::ProvisionPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::ProvisionPermissionSetAsync(const ProvisionPermissionSetRequest& request, const ProvisionPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::ProvisionPermissionSet, this, request, handler, context, m_executor.get());
}

PutInlinePolicyToPermissionSetOutcome SSOAdminClient::PutInlinePolicyToPermissionSet(const PutInlinePolicyToPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutInlinePolicyToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutInlinePolicyToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutInlinePolicyToPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutInlinePolicyToPermissionSetOutcomeCallable SSOAdminClient::PutInlinePolicyToPermissionSetCallable(const PutInlinePolicyToPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::PutInlinePolicyToPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::PutInlinePolicyToPermissionSetAsync(const PutInlinePolicyToPermissionSetRequest& request, const PutInlinePolicyToPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::PutInlinePolicyToPermissionSet, this, request, handler, context, m_executor.get());
}

PutPermissionsBoundaryToPermissionSetOutcome SSOAdminClient::PutPermissionsBoundaryToPermissionSet(const PutPermissionsBoundaryToPermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutPermissionsBoundaryToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutPermissionsBoundaryToPermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PutPermissionsBoundaryToPermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutPermissionsBoundaryToPermissionSetOutcomeCallable SSOAdminClient::PutPermissionsBoundaryToPermissionSetCallable(const PutPermissionsBoundaryToPermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::PutPermissionsBoundaryToPermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::PutPermissionsBoundaryToPermissionSetAsync(const PutPermissionsBoundaryToPermissionSetRequest& request, const PutPermissionsBoundaryToPermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::PutPermissionsBoundaryToPermissionSet, this, request, handler, context, m_executor.get());
}

TagResourceOutcome SSOAdminClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable SSOAdminClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::TagResource, this, request, m_executor.get());
}

void SSOAdminClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome SSOAdminClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable SSOAdminClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::UntagResource, this, request, m_executor.get());
}

void SSOAdminClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateInstanceAccessControlAttributeConfigurationOutcome SSOAdminClient::UpdateInstanceAccessControlAttributeConfiguration(const UpdateInstanceAccessControlAttributeConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateInstanceAccessControlAttributeConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateInstanceAccessControlAttributeConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateInstanceAccessControlAttributeConfigurationOutcomeCallable SSOAdminClient::UpdateInstanceAccessControlAttributeConfigurationCallable(const UpdateInstanceAccessControlAttributeConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::UpdateInstanceAccessControlAttributeConfiguration, this, request, m_executor.get());
}

void SSOAdminClient::UpdateInstanceAccessControlAttributeConfigurationAsync(const UpdateInstanceAccessControlAttributeConfigurationRequest& request, const UpdateInstanceAccessControlAttributeConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::UpdateInstanceAccessControlAttributeConfiguration, this, request, handler, context, m_executor.get());
}

UpdatePermissionSetOutcome SSOAdminClient::UpdatePermissionSet(const UpdatePermissionSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdatePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdatePermissionSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdatePermissionSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdatePermissionSetOutcomeCallable SSOAdminClient::UpdatePermissionSetCallable(const UpdatePermissionSetRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &SSOAdminClient::UpdatePermissionSet, this, request, m_executor.get());
}

void SSOAdminClient::UpdatePermissionSetAsync(const UpdatePermissionSetRequest& request, const UpdatePermissionSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&SSOAdminClient::UpdatePermissionSet, this, request, handler, context, m_executor.get());
}

