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

#include <aws/cognito-idp/CognitoIdentityProviderClient.h>
#include <aws/cognito-idp/CognitoIdentityProviderErrorMarshaller.h>
#include <aws/cognito-idp/CognitoIdentityProviderEndpointProvider.h>
#include <aws/cognito-idp/model/AddCustomAttributesRequest.h>
#include <aws/cognito-idp/model/AdminAddUserToGroupRequest.h>
#include <aws/cognito-idp/model/AdminConfirmSignUpRequest.h>
#include <aws/cognito-idp/model/AdminCreateUserRequest.h>
#include <aws/cognito-idp/model/AdminDeleteUserRequest.h>
#include <aws/cognito-idp/model/AdminDeleteUserAttributesRequest.h>
#include <aws/cognito-idp/model/AdminDisableProviderForUserRequest.h>
#include <aws/cognito-idp/model/AdminDisableUserRequest.h>
#include <aws/cognito-idp/model/AdminEnableUserRequest.h>
#include <aws/cognito-idp/model/AdminForgetDeviceRequest.h>
#include <aws/cognito-idp/model/AdminGetDeviceRequest.h>
#include <aws/cognito-idp/model/AdminGetUserRequest.h>
#include <aws/cognito-idp/model/AdminInitiateAuthRequest.h>
#include <aws/cognito-idp/model/AdminLinkProviderForUserRequest.h>
#include <aws/cognito-idp/model/AdminListDevicesRequest.h>
#include <aws/cognito-idp/model/AdminListGroupsForUserRequest.h>
#include <aws/cognito-idp/model/AdminListUserAuthEventsRequest.h>
#include <aws/cognito-idp/model/AdminRemoveUserFromGroupRequest.h>
#include <aws/cognito-idp/model/AdminResetUserPasswordRequest.h>
#include <aws/cognito-idp/model/AdminRespondToAuthChallengeRequest.h>
#include <aws/cognito-idp/model/AdminSetUserMFAPreferenceRequest.h>
#include <aws/cognito-idp/model/AdminSetUserPasswordRequest.h>
#include <aws/cognito-idp/model/AdminSetUserSettingsRequest.h>
#include <aws/cognito-idp/model/AdminUpdateAuthEventFeedbackRequest.h>
#include <aws/cognito-idp/model/AdminUpdateDeviceStatusRequest.h>
#include <aws/cognito-idp/model/AdminUpdateUserAttributesRequest.h>
#include <aws/cognito-idp/model/AdminUserGlobalSignOutRequest.h>
#include <aws/cognito-idp/model/AssociateSoftwareTokenRequest.h>
#include <aws/cognito-idp/model/ChangePasswordRequest.h>
#include <aws/cognito-idp/model/ConfirmDeviceRequest.h>
#include <aws/cognito-idp/model/ConfirmForgotPasswordRequest.h>
#include <aws/cognito-idp/model/ConfirmSignUpRequest.h>
#include <aws/cognito-idp/model/CreateGroupRequest.h>
#include <aws/cognito-idp/model/CreateIdentityProviderRequest.h>
#include <aws/cognito-idp/model/CreateResourceServerRequest.h>
#include <aws/cognito-idp/model/CreateUserImportJobRequest.h>
#include <aws/cognito-idp/model/CreateUserPoolRequest.h>
#include <aws/cognito-idp/model/CreateUserPoolClientRequest.h>
#include <aws/cognito-idp/model/CreateUserPoolDomainRequest.h>
#include <aws/cognito-idp/model/DeleteGroupRequest.h>
#include <aws/cognito-idp/model/DeleteIdentityProviderRequest.h>
#include <aws/cognito-idp/model/DeleteResourceServerRequest.h>
#include <aws/cognito-idp/model/DeleteUserRequest.h>
#include <aws/cognito-idp/model/DeleteUserAttributesRequest.h>
#include <aws/cognito-idp/model/DeleteUserPoolRequest.h>
#include <aws/cognito-idp/model/DeleteUserPoolClientRequest.h>
#include <aws/cognito-idp/model/DeleteUserPoolDomainRequest.h>
#include <aws/cognito-idp/model/DescribeIdentityProviderRequest.h>
#include <aws/cognito-idp/model/DescribeResourceServerRequest.h>
#include <aws/cognito-idp/model/DescribeRiskConfigurationRequest.h>
#include <aws/cognito-idp/model/DescribeUserImportJobRequest.h>
#include <aws/cognito-idp/model/DescribeUserPoolRequest.h>
#include <aws/cognito-idp/model/DescribeUserPoolClientRequest.h>
#include <aws/cognito-idp/model/DescribeUserPoolDomainRequest.h>
#include <aws/cognito-idp/model/ForgetDeviceRequest.h>
#include <aws/cognito-idp/model/ForgotPasswordRequest.h>
#include <aws/cognito-idp/model/GetCSVHeaderRequest.h>
#include <aws/cognito-idp/model/GetDeviceRequest.h>
#include <aws/cognito-idp/model/GetGroupRequest.h>
#include <aws/cognito-idp/model/GetIdentityProviderByIdentifierRequest.h>
#include <aws/cognito-idp/model/GetSigningCertificateRequest.h>
#include <aws/cognito-idp/model/GetUICustomizationRequest.h>
#include <aws/cognito-idp/model/GetUserRequest.h>
#include <aws/cognito-idp/model/GetUserAttributeVerificationCodeRequest.h>
#include <aws/cognito-idp/model/GetUserPoolMfaConfigRequest.h>
#include <aws/cognito-idp/model/GlobalSignOutRequest.h>
#include <aws/cognito-idp/model/InitiateAuthRequest.h>
#include <aws/cognito-idp/model/ListDevicesRequest.h>
#include <aws/cognito-idp/model/ListGroupsRequest.h>
#include <aws/cognito-idp/model/ListIdentityProvidersRequest.h>
#include <aws/cognito-idp/model/ListResourceServersRequest.h>
#include <aws/cognito-idp/model/ListTagsForResourceRequest.h>
#include <aws/cognito-idp/model/ListUserImportJobsRequest.h>
#include <aws/cognito-idp/model/ListUserPoolClientsRequest.h>
#include <aws/cognito-idp/model/ListUserPoolsRequest.h>
#include <aws/cognito-idp/model/ListUsersRequest.h>
#include <aws/cognito-idp/model/ListUsersInGroupRequest.h>
#include <aws/cognito-idp/model/ResendConfirmationCodeRequest.h>
#include <aws/cognito-idp/model/RespondToAuthChallengeRequest.h>
#include <aws/cognito-idp/model/RevokeTokenRequest.h>
#include <aws/cognito-idp/model/SetRiskConfigurationRequest.h>
#include <aws/cognito-idp/model/SetUICustomizationRequest.h>
#include <aws/cognito-idp/model/SetUserMFAPreferenceRequest.h>
#include <aws/cognito-idp/model/SetUserPoolMfaConfigRequest.h>
#include <aws/cognito-idp/model/SetUserSettingsRequest.h>
#include <aws/cognito-idp/model/SignUpRequest.h>
#include <aws/cognito-idp/model/StartUserImportJobRequest.h>
#include <aws/cognito-idp/model/StopUserImportJobRequest.h>
#include <aws/cognito-idp/model/TagResourceRequest.h>
#include <aws/cognito-idp/model/UntagResourceRequest.h>
#include <aws/cognito-idp/model/UpdateAuthEventFeedbackRequest.h>
#include <aws/cognito-idp/model/UpdateDeviceStatusRequest.h>
#include <aws/cognito-idp/model/UpdateGroupRequest.h>
#include <aws/cognito-idp/model/UpdateIdentityProviderRequest.h>
#include <aws/cognito-idp/model/UpdateResourceServerRequest.h>
#include <aws/cognito-idp/model/UpdateUserAttributesRequest.h>
#include <aws/cognito-idp/model/UpdateUserPoolRequest.h>
#include <aws/cognito-idp/model/UpdateUserPoolClientRequest.h>
#include <aws/cognito-idp/model/UpdateUserPoolDomainRequest.h>
#include <aws/cognito-idp/model/VerifySoftwareTokenRequest.h>
#include <aws/cognito-idp/model/VerifyUserAttributeRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::CognitoIdentityProvider;
using namespace Aws::CognitoIdentityProvider::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* CognitoIdentityProviderClient::SERVICE_NAME = "cognito-idp";
const char* CognitoIdentityProviderClient::ALLOCATION_TAG = "CognitoIdentityProviderClient";

CognitoIdentityProviderClient::CognitoIdentityProviderClient(const CognitoIdentityProvider::CognitoIdentityProviderClientConfiguration& clientConfiguration,
                                                             std::shared_ptr<CognitoIdentityProviderEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CognitoIdentityProviderClient::CognitoIdentityProviderClient(const AWSCredentials& credentials,
                                                             std::shared_ptr<CognitoIdentityProviderEndpointProviderBase> endpointProvider,
                                                             const CognitoIdentityProvider::CognitoIdentityProviderClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

CognitoIdentityProviderClient::CognitoIdentityProviderClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                             std::shared_ptr<CognitoIdentityProviderEndpointProviderBase> endpointProvider,
                                                             const CognitoIdentityProvider::CognitoIdentityProviderClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  CognitoIdentityProviderClient::CognitoIdentityProviderClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<CognitoIdentityProviderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CognitoIdentityProviderClient::CognitoIdentityProviderClient(const AWSCredentials& credentials,
                                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CognitoIdentityProviderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

CognitoIdentityProviderClient::CognitoIdentityProviderClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<CognitoIdentityProviderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<CognitoIdentityProviderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
CognitoIdentityProviderClient::~CognitoIdentityProviderClient()
{
}

std::shared_ptr<CognitoIdentityProviderEndpointProviderBase>& CognitoIdentityProviderClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void CognitoIdentityProviderClient::init(const CognitoIdentityProvider::CognitoIdentityProviderClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Cognito Identity Provider");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void CognitoIdentityProviderClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AddCustomAttributesOutcome CognitoIdentityProviderClient::AddCustomAttributes(const AddCustomAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddCustomAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddCustomAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddCustomAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AddCustomAttributesOutcomeCallable CognitoIdentityProviderClient::AddCustomAttributesCallable(const AddCustomAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AddCustomAttributes, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AddCustomAttributesAsync(const AddCustomAttributesRequest& request, const AddCustomAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AddCustomAttributes, this, request, handler, context, m_executor.get());
}

AdminAddUserToGroupOutcome CognitoIdentityProviderClient::AdminAddUserToGroup(const AdminAddUserToGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminAddUserToGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminAddUserToGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminAddUserToGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminAddUserToGroupOutcomeCallable CognitoIdentityProviderClient::AdminAddUserToGroupCallable(const AdminAddUserToGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminAddUserToGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminAddUserToGroupAsync(const AdminAddUserToGroupRequest& request, const AdminAddUserToGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminAddUserToGroup, this, request, handler, context, m_executor.get());
}

AdminConfirmSignUpOutcome CognitoIdentityProviderClient::AdminConfirmSignUp(const AdminConfirmSignUpRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminConfirmSignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminConfirmSignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminConfirmSignUpOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminConfirmSignUpOutcomeCallable CognitoIdentityProviderClient::AdminConfirmSignUpCallable(const AdminConfirmSignUpRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminConfirmSignUp, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminConfirmSignUpAsync(const AdminConfirmSignUpRequest& request, const AdminConfirmSignUpResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminConfirmSignUp, this, request, handler, context, m_executor.get());
}

AdminCreateUserOutcome CognitoIdentityProviderClient::AdminCreateUser(const AdminCreateUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminCreateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminCreateUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminCreateUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminCreateUserOutcomeCallable CognitoIdentityProviderClient::AdminCreateUserCallable(const AdminCreateUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminCreateUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminCreateUserAsync(const AdminCreateUserRequest& request, const AdminCreateUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminCreateUser, this, request, handler, context, m_executor.get());
}

AdminDeleteUserOutcome CognitoIdentityProviderClient::AdminDeleteUser(const AdminDeleteUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminDeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminDeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminDeleteUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminDeleteUserOutcomeCallable CognitoIdentityProviderClient::AdminDeleteUserCallable(const AdminDeleteUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminDeleteUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminDeleteUserAsync(const AdminDeleteUserRequest& request, const AdminDeleteUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminDeleteUser, this, request, handler, context, m_executor.get());
}

AdminDeleteUserAttributesOutcome CognitoIdentityProviderClient::AdminDeleteUserAttributes(const AdminDeleteUserAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminDeleteUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminDeleteUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminDeleteUserAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminDeleteUserAttributesOutcomeCallable CognitoIdentityProviderClient::AdminDeleteUserAttributesCallable(const AdminDeleteUserAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminDeleteUserAttributes, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminDeleteUserAttributesAsync(const AdminDeleteUserAttributesRequest& request, const AdminDeleteUserAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminDeleteUserAttributes, this, request, handler, context, m_executor.get());
}

AdminDisableProviderForUserOutcome CognitoIdentityProviderClient::AdminDisableProviderForUser(const AdminDisableProviderForUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminDisableProviderForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminDisableProviderForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminDisableProviderForUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminDisableProviderForUserOutcomeCallable CognitoIdentityProviderClient::AdminDisableProviderForUserCallable(const AdminDisableProviderForUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminDisableProviderForUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminDisableProviderForUserAsync(const AdminDisableProviderForUserRequest& request, const AdminDisableProviderForUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminDisableProviderForUser, this, request, handler, context, m_executor.get());
}

AdminDisableUserOutcome CognitoIdentityProviderClient::AdminDisableUser(const AdminDisableUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminDisableUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminDisableUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminDisableUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminDisableUserOutcomeCallable CognitoIdentityProviderClient::AdminDisableUserCallable(const AdminDisableUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminDisableUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminDisableUserAsync(const AdminDisableUserRequest& request, const AdminDisableUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminDisableUser, this, request, handler, context, m_executor.get());
}

AdminEnableUserOutcome CognitoIdentityProviderClient::AdminEnableUser(const AdminEnableUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminEnableUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminEnableUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminEnableUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminEnableUserOutcomeCallable CognitoIdentityProviderClient::AdminEnableUserCallable(const AdminEnableUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminEnableUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminEnableUserAsync(const AdminEnableUserRequest& request, const AdminEnableUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminEnableUser, this, request, handler, context, m_executor.get());
}

AdminForgetDeviceOutcome CognitoIdentityProviderClient::AdminForgetDevice(const AdminForgetDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminForgetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminForgetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminForgetDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminForgetDeviceOutcomeCallable CognitoIdentityProviderClient::AdminForgetDeviceCallable(const AdminForgetDeviceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminForgetDevice, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminForgetDeviceAsync(const AdminForgetDeviceRequest& request, const AdminForgetDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminForgetDevice, this, request, handler, context, m_executor.get());
}

AdminGetDeviceOutcome CognitoIdentityProviderClient::AdminGetDevice(const AdminGetDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminGetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminGetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminGetDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminGetDeviceOutcomeCallable CognitoIdentityProviderClient::AdminGetDeviceCallable(const AdminGetDeviceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminGetDevice, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminGetDeviceAsync(const AdminGetDeviceRequest& request, const AdminGetDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminGetDevice, this, request, handler, context, m_executor.get());
}

AdminGetUserOutcome CognitoIdentityProviderClient::AdminGetUser(const AdminGetUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminGetUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminGetUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminGetUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminGetUserOutcomeCallable CognitoIdentityProviderClient::AdminGetUserCallable(const AdminGetUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminGetUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminGetUserAsync(const AdminGetUserRequest& request, const AdminGetUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminGetUser, this, request, handler, context, m_executor.get());
}

AdminInitiateAuthOutcome CognitoIdentityProviderClient::AdminInitiateAuth(const AdminInitiateAuthRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminInitiateAuth, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminInitiateAuth, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminInitiateAuthOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminInitiateAuthOutcomeCallable CognitoIdentityProviderClient::AdminInitiateAuthCallable(const AdminInitiateAuthRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminInitiateAuth, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminInitiateAuthAsync(const AdminInitiateAuthRequest& request, const AdminInitiateAuthResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminInitiateAuth, this, request, handler, context, m_executor.get());
}

AdminLinkProviderForUserOutcome CognitoIdentityProviderClient::AdminLinkProviderForUser(const AdminLinkProviderForUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminLinkProviderForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminLinkProviderForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminLinkProviderForUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminLinkProviderForUserOutcomeCallable CognitoIdentityProviderClient::AdminLinkProviderForUserCallable(const AdminLinkProviderForUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminLinkProviderForUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminLinkProviderForUserAsync(const AdminLinkProviderForUserRequest& request, const AdminLinkProviderForUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminLinkProviderForUser, this, request, handler, context, m_executor.get());
}

AdminListDevicesOutcome CognitoIdentityProviderClient::AdminListDevices(const AdminListDevicesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminListDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminListDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminListDevicesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminListDevicesOutcomeCallable CognitoIdentityProviderClient::AdminListDevicesCallable(const AdminListDevicesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminListDevices, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminListDevicesAsync(const AdminListDevicesRequest& request, const AdminListDevicesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminListDevices, this, request, handler, context, m_executor.get());
}

AdminListGroupsForUserOutcome CognitoIdentityProviderClient::AdminListGroupsForUser(const AdminListGroupsForUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminListGroupsForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminListGroupsForUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminListGroupsForUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminListGroupsForUserOutcomeCallable CognitoIdentityProviderClient::AdminListGroupsForUserCallable(const AdminListGroupsForUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminListGroupsForUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminListGroupsForUserAsync(const AdminListGroupsForUserRequest& request, const AdminListGroupsForUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminListGroupsForUser, this, request, handler, context, m_executor.get());
}

AdminListUserAuthEventsOutcome CognitoIdentityProviderClient::AdminListUserAuthEvents(const AdminListUserAuthEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminListUserAuthEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminListUserAuthEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminListUserAuthEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminListUserAuthEventsOutcomeCallable CognitoIdentityProviderClient::AdminListUserAuthEventsCallable(const AdminListUserAuthEventsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminListUserAuthEvents, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminListUserAuthEventsAsync(const AdminListUserAuthEventsRequest& request, const AdminListUserAuthEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminListUserAuthEvents, this, request, handler, context, m_executor.get());
}

AdminRemoveUserFromGroupOutcome CognitoIdentityProviderClient::AdminRemoveUserFromGroup(const AdminRemoveUserFromGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminRemoveUserFromGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminRemoveUserFromGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminRemoveUserFromGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminRemoveUserFromGroupOutcomeCallable CognitoIdentityProviderClient::AdminRemoveUserFromGroupCallable(const AdminRemoveUserFromGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminRemoveUserFromGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminRemoveUserFromGroupAsync(const AdminRemoveUserFromGroupRequest& request, const AdminRemoveUserFromGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminRemoveUserFromGroup, this, request, handler, context, m_executor.get());
}

AdminResetUserPasswordOutcome CognitoIdentityProviderClient::AdminResetUserPassword(const AdminResetUserPasswordRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminResetUserPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminResetUserPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminResetUserPasswordOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminResetUserPasswordOutcomeCallable CognitoIdentityProviderClient::AdminResetUserPasswordCallable(const AdminResetUserPasswordRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminResetUserPassword, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminResetUserPasswordAsync(const AdminResetUserPasswordRequest& request, const AdminResetUserPasswordResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminResetUserPassword, this, request, handler, context, m_executor.get());
}

AdminRespondToAuthChallengeOutcome CognitoIdentityProviderClient::AdminRespondToAuthChallenge(const AdminRespondToAuthChallengeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminRespondToAuthChallenge, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminRespondToAuthChallenge, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminRespondToAuthChallengeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminRespondToAuthChallengeOutcomeCallable CognitoIdentityProviderClient::AdminRespondToAuthChallengeCallable(const AdminRespondToAuthChallengeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminRespondToAuthChallenge, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminRespondToAuthChallengeAsync(const AdminRespondToAuthChallengeRequest& request, const AdminRespondToAuthChallengeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminRespondToAuthChallenge, this, request, handler, context, m_executor.get());
}

AdminSetUserMFAPreferenceOutcome CognitoIdentityProviderClient::AdminSetUserMFAPreference(const AdminSetUserMFAPreferenceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminSetUserMFAPreference, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminSetUserMFAPreference, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminSetUserMFAPreferenceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminSetUserMFAPreferenceOutcomeCallable CognitoIdentityProviderClient::AdminSetUserMFAPreferenceCallable(const AdminSetUserMFAPreferenceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminSetUserMFAPreference, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminSetUserMFAPreferenceAsync(const AdminSetUserMFAPreferenceRequest& request, const AdminSetUserMFAPreferenceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminSetUserMFAPreference, this, request, handler, context, m_executor.get());
}

AdminSetUserPasswordOutcome CognitoIdentityProviderClient::AdminSetUserPassword(const AdminSetUserPasswordRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminSetUserPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminSetUserPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminSetUserPasswordOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminSetUserPasswordOutcomeCallable CognitoIdentityProviderClient::AdminSetUserPasswordCallable(const AdminSetUserPasswordRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminSetUserPassword, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminSetUserPasswordAsync(const AdminSetUserPasswordRequest& request, const AdminSetUserPasswordResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminSetUserPassword, this, request, handler, context, m_executor.get());
}

AdminSetUserSettingsOutcome CognitoIdentityProviderClient::AdminSetUserSettings(const AdminSetUserSettingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminSetUserSettings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminSetUserSettings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminSetUserSettingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminSetUserSettingsOutcomeCallable CognitoIdentityProviderClient::AdminSetUserSettingsCallable(const AdminSetUserSettingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminSetUserSettings, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminSetUserSettingsAsync(const AdminSetUserSettingsRequest& request, const AdminSetUserSettingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminSetUserSettings, this, request, handler, context, m_executor.get());
}

AdminUpdateAuthEventFeedbackOutcome CognitoIdentityProviderClient::AdminUpdateAuthEventFeedback(const AdminUpdateAuthEventFeedbackRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminUpdateAuthEventFeedback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminUpdateAuthEventFeedback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminUpdateAuthEventFeedbackOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminUpdateAuthEventFeedbackOutcomeCallable CognitoIdentityProviderClient::AdminUpdateAuthEventFeedbackCallable(const AdminUpdateAuthEventFeedbackRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminUpdateAuthEventFeedback, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminUpdateAuthEventFeedbackAsync(const AdminUpdateAuthEventFeedbackRequest& request, const AdminUpdateAuthEventFeedbackResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminUpdateAuthEventFeedback, this, request, handler, context, m_executor.get());
}

AdminUpdateDeviceStatusOutcome CognitoIdentityProviderClient::AdminUpdateDeviceStatus(const AdminUpdateDeviceStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminUpdateDeviceStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminUpdateDeviceStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminUpdateDeviceStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminUpdateDeviceStatusOutcomeCallable CognitoIdentityProviderClient::AdminUpdateDeviceStatusCallable(const AdminUpdateDeviceStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminUpdateDeviceStatus, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminUpdateDeviceStatusAsync(const AdminUpdateDeviceStatusRequest& request, const AdminUpdateDeviceStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminUpdateDeviceStatus, this, request, handler, context, m_executor.get());
}

AdminUpdateUserAttributesOutcome CognitoIdentityProviderClient::AdminUpdateUserAttributes(const AdminUpdateUserAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminUpdateUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminUpdateUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminUpdateUserAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminUpdateUserAttributesOutcomeCallable CognitoIdentityProviderClient::AdminUpdateUserAttributesCallable(const AdminUpdateUserAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminUpdateUserAttributes, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminUpdateUserAttributesAsync(const AdminUpdateUserAttributesRequest& request, const AdminUpdateUserAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminUpdateUserAttributes, this, request, handler, context, m_executor.get());
}

AdminUserGlobalSignOutOutcome CognitoIdentityProviderClient::AdminUserGlobalSignOut(const AdminUserGlobalSignOutRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AdminUserGlobalSignOut, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AdminUserGlobalSignOut, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AdminUserGlobalSignOutOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AdminUserGlobalSignOutOutcomeCallable CognitoIdentityProviderClient::AdminUserGlobalSignOutCallable(const AdminUserGlobalSignOutRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AdminUserGlobalSignOut, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AdminUserGlobalSignOutAsync(const AdminUserGlobalSignOutRequest& request, const AdminUserGlobalSignOutResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AdminUserGlobalSignOut, this, request, handler, context, m_executor.get());
}

AssociateSoftwareTokenOutcome CognitoIdentityProviderClient::AssociateSoftwareToken(const AssociateSoftwareTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateSoftwareToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateSoftwareToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateSoftwareTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateSoftwareTokenOutcomeCallable CognitoIdentityProviderClient::AssociateSoftwareTokenCallable(const AssociateSoftwareTokenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::AssociateSoftwareToken, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::AssociateSoftwareTokenAsync(const AssociateSoftwareTokenRequest& request, const AssociateSoftwareTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::AssociateSoftwareToken, this, request, handler, context, m_executor.get());
}

ChangePasswordOutcome CognitoIdentityProviderClient::ChangePassword(const ChangePasswordRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ChangePassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ChangePassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ChangePasswordOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

ChangePasswordOutcomeCallable CognitoIdentityProviderClient::ChangePasswordCallable(const ChangePasswordRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ChangePassword, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ChangePasswordAsync(const ChangePasswordRequest& request, const ChangePasswordResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ChangePassword, this, request, handler, context, m_executor.get());
}

ConfirmDeviceOutcome CognitoIdentityProviderClient::ConfirmDevice(const ConfirmDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ConfirmDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ConfirmDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ConfirmDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ConfirmDeviceOutcomeCallable CognitoIdentityProviderClient::ConfirmDeviceCallable(const ConfirmDeviceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ConfirmDevice, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ConfirmDeviceAsync(const ConfirmDeviceRequest& request, const ConfirmDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ConfirmDevice, this, request, handler, context, m_executor.get());
}

ConfirmForgotPasswordOutcome CognitoIdentityProviderClient::ConfirmForgotPassword(const ConfirmForgotPasswordRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ConfirmForgotPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ConfirmForgotPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ConfirmForgotPasswordOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

ConfirmForgotPasswordOutcomeCallable CognitoIdentityProviderClient::ConfirmForgotPasswordCallable(const ConfirmForgotPasswordRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ConfirmForgotPassword, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ConfirmForgotPasswordAsync(const ConfirmForgotPasswordRequest& request, const ConfirmForgotPasswordResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ConfirmForgotPassword, this, request, handler, context, m_executor.get());
}

ConfirmSignUpOutcome CognitoIdentityProviderClient::ConfirmSignUp(const ConfirmSignUpRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ConfirmSignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ConfirmSignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ConfirmSignUpOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

ConfirmSignUpOutcomeCallable CognitoIdentityProviderClient::ConfirmSignUpCallable(const ConfirmSignUpRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ConfirmSignUp, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ConfirmSignUpAsync(const ConfirmSignUpRequest& request, const ConfirmSignUpResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ConfirmSignUp, this, request, handler, context, m_executor.get());
}

CreateGroupOutcome CognitoIdentityProviderClient::CreateGroup(const CreateGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGroupOutcomeCallable CognitoIdentityProviderClient::CreateGroupCallable(const CreateGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateGroupAsync(const CreateGroupRequest& request, const CreateGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateGroup, this, request, handler, context, m_executor.get());
}

CreateIdentityProviderOutcome CognitoIdentityProviderClient::CreateIdentityProvider(const CreateIdentityProviderRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateIdentityProviderOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateIdentityProviderOutcomeCallable CognitoIdentityProviderClient::CreateIdentityProviderCallable(const CreateIdentityProviderRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateIdentityProvider, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateIdentityProviderAsync(const CreateIdentityProviderRequest& request, const CreateIdentityProviderResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateIdentityProvider, this, request, handler, context, m_executor.get());
}

CreateResourceServerOutcome CognitoIdentityProviderClient::CreateResourceServer(const CreateResourceServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateResourceServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateResourceServerOutcomeCallable CognitoIdentityProviderClient::CreateResourceServerCallable(const CreateResourceServerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateResourceServer, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateResourceServerAsync(const CreateResourceServerRequest& request, const CreateResourceServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateResourceServer, this, request, handler, context, m_executor.get());
}

CreateUserImportJobOutcome CognitoIdentityProviderClient::CreateUserImportJob(const CreateUserImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUserImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUserImportJobOutcomeCallable CognitoIdentityProviderClient::CreateUserImportJobCallable(const CreateUserImportJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateUserImportJob, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateUserImportJobAsync(const CreateUserImportJobRequest& request, const CreateUserImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateUserImportJob, this, request, handler, context, m_executor.get());
}

CreateUserPoolOutcome CognitoIdentityProviderClient::CreateUserPool(const CreateUserPoolRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUserPoolOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUserPoolOutcomeCallable CognitoIdentityProviderClient::CreateUserPoolCallable(const CreateUserPoolRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateUserPool, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateUserPoolAsync(const CreateUserPoolRequest& request, const CreateUserPoolResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateUserPool, this, request, handler, context, m_executor.get());
}

CreateUserPoolClientOutcome CognitoIdentityProviderClient::CreateUserPoolClient(const CreateUserPoolClientRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUserPoolClientOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUserPoolClientOutcomeCallable CognitoIdentityProviderClient::CreateUserPoolClientCallable(const CreateUserPoolClientRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateUserPoolClient, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateUserPoolClientAsync(const CreateUserPoolClientRequest& request, const CreateUserPoolClientResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateUserPoolClient, this, request, handler, context, m_executor.get());
}

CreateUserPoolDomainOutcome CognitoIdentityProviderClient::CreateUserPoolDomain(const CreateUserPoolDomainRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUserPoolDomainOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateUserPoolDomainOutcomeCallable CognitoIdentityProviderClient::CreateUserPoolDomainCallable(const CreateUserPoolDomainRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::CreateUserPoolDomain, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::CreateUserPoolDomainAsync(const CreateUserPoolDomainRequest& request, const CreateUserPoolDomainResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::CreateUserPoolDomain, this, request, handler, context, m_executor.get());
}

DeleteGroupOutcome CognitoIdentityProviderClient::DeleteGroup(const DeleteGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteGroupOutcomeCallable CognitoIdentityProviderClient::DeleteGroupCallable(const DeleteGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteGroupAsync(const DeleteGroupRequest& request, const DeleteGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteGroup, this, request, handler, context, m_executor.get());
}

DeleteIdentityProviderOutcome CognitoIdentityProviderClient::DeleteIdentityProvider(const DeleteIdentityProviderRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteIdentityProviderOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteIdentityProviderOutcomeCallable CognitoIdentityProviderClient::DeleteIdentityProviderCallable(const DeleteIdentityProviderRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteIdentityProvider, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteIdentityProviderAsync(const DeleteIdentityProviderRequest& request, const DeleteIdentityProviderResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteIdentityProvider, this, request, handler, context, m_executor.get());
}

DeleteResourceServerOutcome CognitoIdentityProviderClient::DeleteResourceServer(const DeleteResourceServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteResourceServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteResourceServerOutcomeCallable CognitoIdentityProviderClient::DeleteResourceServerCallable(const DeleteResourceServerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteResourceServer, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteResourceServerAsync(const DeleteResourceServerRequest& request, const DeleteResourceServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteResourceServer, this, request, handler, context, m_executor.get());
}

DeleteUserOutcome CognitoIdentityProviderClient::DeleteUser(const DeleteUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

DeleteUserOutcomeCallable CognitoIdentityProviderClient::DeleteUserCallable(const DeleteUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteUserAsync(const DeleteUserRequest& request, const DeleteUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteUser, this, request, handler, context, m_executor.get());
}

DeleteUserAttributesOutcome CognitoIdentityProviderClient::DeleteUserAttributes(const DeleteUserAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

DeleteUserAttributesOutcomeCallable CognitoIdentityProviderClient::DeleteUserAttributesCallable(const DeleteUserAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteUserAttributes, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteUserAttributesAsync(const DeleteUserAttributesRequest& request, const DeleteUserAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteUserAttributes, this, request, handler, context, m_executor.get());
}

DeleteUserPoolOutcome CognitoIdentityProviderClient::DeleteUserPool(const DeleteUserPoolRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserPoolOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteUserPoolOutcomeCallable CognitoIdentityProviderClient::DeleteUserPoolCallable(const DeleteUserPoolRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteUserPool, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteUserPoolAsync(const DeleteUserPoolRequest& request, const DeleteUserPoolResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteUserPool, this, request, handler, context, m_executor.get());
}

DeleteUserPoolClientOutcome CognitoIdentityProviderClient::DeleteUserPoolClient(const DeleteUserPoolClientRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserPoolClientOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteUserPoolClientOutcomeCallable CognitoIdentityProviderClient::DeleteUserPoolClientCallable(const DeleteUserPoolClientRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteUserPoolClient, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteUserPoolClientAsync(const DeleteUserPoolClientRequest& request, const DeleteUserPoolClientResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteUserPoolClient, this, request, handler, context, m_executor.get());
}

DeleteUserPoolDomainOutcome CognitoIdentityProviderClient::DeleteUserPoolDomain(const DeleteUserPoolDomainRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUserPoolDomainOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteUserPoolDomainOutcomeCallable CognitoIdentityProviderClient::DeleteUserPoolDomainCallable(const DeleteUserPoolDomainRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DeleteUserPoolDomain, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DeleteUserPoolDomainAsync(const DeleteUserPoolDomainRequest& request, const DeleteUserPoolDomainResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DeleteUserPoolDomain, this, request, handler, context, m_executor.get());
}

DescribeIdentityProviderOutcome CognitoIdentityProviderClient::DescribeIdentityProvider(const DescribeIdentityProviderRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeIdentityProviderOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeIdentityProviderOutcomeCallable CognitoIdentityProviderClient::DescribeIdentityProviderCallable(const DescribeIdentityProviderRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeIdentityProvider, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeIdentityProviderAsync(const DescribeIdentityProviderRequest& request, const DescribeIdentityProviderResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeIdentityProvider, this, request, handler, context, m_executor.get());
}

DescribeResourceServerOutcome CognitoIdentityProviderClient::DescribeResourceServer(const DescribeResourceServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeResourceServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeResourceServerOutcomeCallable CognitoIdentityProviderClient::DescribeResourceServerCallable(const DescribeResourceServerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeResourceServer, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeResourceServerAsync(const DescribeResourceServerRequest& request, const DescribeResourceServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeResourceServer, this, request, handler, context, m_executor.get());
}

DescribeRiskConfigurationOutcome CognitoIdentityProviderClient::DescribeRiskConfiguration(const DescribeRiskConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeRiskConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeRiskConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeRiskConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeRiskConfigurationOutcomeCallable CognitoIdentityProviderClient::DescribeRiskConfigurationCallable(const DescribeRiskConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeRiskConfiguration, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeRiskConfigurationAsync(const DescribeRiskConfigurationRequest& request, const DescribeRiskConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeRiskConfiguration, this, request, handler, context, m_executor.get());
}

DescribeUserImportJobOutcome CognitoIdentityProviderClient::DescribeUserImportJob(const DescribeUserImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUserImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeUserImportJobOutcomeCallable CognitoIdentityProviderClient::DescribeUserImportJobCallable(const DescribeUserImportJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeUserImportJob, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeUserImportJobAsync(const DescribeUserImportJobRequest& request, const DescribeUserImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeUserImportJob, this, request, handler, context, m_executor.get());
}

DescribeUserPoolOutcome CognitoIdentityProviderClient::DescribeUserPool(const DescribeUserPoolRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUserPoolOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeUserPoolOutcomeCallable CognitoIdentityProviderClient::DescribeUserPoolCallable(const DescribeUserPoolRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeUserPool, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeUserPoolAsync(const DescribeUserPoolRequest& request, const DescribeUserPoolResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeUserPool, this, request, handler, context, m_executor.get());
}

DescribeUserPoolClientOutcome CognitoIdentityProviderClient::DescribeUserPoolClient(const DescribeUserPoolClientRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUserPoolClientOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeUserPoolClientOutcomeCallable CognitoIdentityProviderClient::DescribeUserPoolClientCallable(const DescribeUserPoolClientRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeUserPoolClient, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeUserPoolClientAsync(const DescribeUserPoolClientRequest& request, const DescribeUserPoolClientResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeUserPoolClient, this, request, handler, context, m_executor.get());
}

DescribeUserPoolDomainOutcome CognitoIdentityProviderClient::DescribeUserPoolDomain(const DescribeUserPoolDomainRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUserPoolDomainOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeUserPoolDomainOutcomeCallable CognitoIdentityProviderClient::DescribeUserPoolDomainCallable(const DescribeUserPoolDomainRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::DescribeUserPoolDomain, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::DescribeUserPoolDomainAsync(const DescribeUserPoolDomainRequest& request, const DescribeUserPoolDomainResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::DescribeUserPoolDomain, this, request, handler, context, m_executor.get());
}

ForgetDeviceOutcome CognitoIdentityProviderClient::ForgetDevice(const ForgetDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ForgetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ForgetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ForgetDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ForgetDeviceOutcomeCallable CognitoIdentityProviderClient::ForgetDeviceCallable(const ForgetDeviceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ForgetDevice, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ForgetDeviceAsync(const ForgetDeviceRequest& request, const ForgetDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ForgetDevice, this, request, handler, context, m_executor.get());
}

ForgotPasswordOutcome CognitoIdentityProviderClient::ForgotPassword(const ForgotPasswordRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ForgotPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ForgotPassword, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ForgotPasswordOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

ForgotPasswordOutcomeCallable CognitoIdentityProviderClient::ForgotPasswordCallable(const ForgotPasswordRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ForgotPassword, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ForgotPasswordAsync(const ForgotPasswordRequest& request, const ForgotPasswordResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ForgotPassword, this, request, handler, context, m_executor.get());
}

GetCSVHeaderOutcome CognitoIdentityProviderClient::GetCSVHeader(const GetCSVHeaderRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCSVHeader, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCSVHeader, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetCSVHeaderOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetCSVHeaderOutcomeCallable CognitoIdentityProviderClient::GetCSVHeaderCallable(const GetCSVHeaderRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetCSVHeader, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetCSVHeaderAsync(const GetCSVHeaderRequest& request, const GetCSVHeaderResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetCSVHeader, this, request, handler, context, m_executor.get());
}

GetDeviceOutcome CognitoIdentityProviderClient::GetDevice(const GetDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetDeviceOutcomeCallable CognitoIdentityProviderClient::GetDeviceCallable(const GetDeviceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetDevice, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetDeviceAsync(const GetDeviceRequest& request, const GetDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetDevice, this, request, handler, context, m_executor.get());
}

GetGroupOutcome CognitoIdentityProviderClient::GetGroup(const GetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetGroupOutcomeCallable CognitoIdentityProviderClient::GetGroupCallable(const GetGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetGroupAsync(const GetGroupRequest& request, const GetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetGroup, this, request, handler, context, m_executor.get());
}

GetIdentityProviderByIdentifierOutcome CognitoIdentityProviderClient::GetIdentityProviderByIdentifier(const GetIdentityProviderByIdentifierRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIdentityProviderByIdentifier, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIdentityProviderByIdentifier, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetIdentityProviderByIdentifierOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetIdentityProviderByIdentifierOutcomeCallable CognitoIdentityProviderClient::GetIdentityProviderByIdentifierCallable(const GetIdentityProviderByIdentifierRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetIdentityProviderByIdentifier, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetIdentityProviderByIdentifierAsync(const GetIdentityProviderByIdentifierRequest& request, const GetIdentityProviderByIdentifierResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetIdentityProviderByIdentifier, this, request, handler, context, m_executor.get());
}

GetSigningCertificateOutcome CognitoIdentityProviderClient::GetSigningCertificate(const GetSigningCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSigningCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSigningCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetSigningCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetSigningCertificateOutcomeCallable CognitoIdentityProviderClient::GetSigningCertificateCallable(const GetSigningCertificateRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetSigningCertificate, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetSigningCertificateAsync(const GetSigningCertificateRequest& request, const GetSigningCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetSigningCertificate, this, request, handler, context, m_executor.get());
}

GetUICustomizationOutcome CognitoIdentityProviderClient::GetUICustomization(const GetUICustomizationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUICustomization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUICustomization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetUICustomizationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetUICustomizationOutcomeCallable CognitoIdentityProviderClient::GetUICustomizationCallable(const GetUICustomizationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetUICustomization, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetUICustomizationAsync(const GetUICustomizationRequest& request, const GetUICustomizationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetUICustomization, this, request, handler, context, m_executor.get());
}

GetUserOutcome CognitoIdentityProviderClient::GetUser(const GetUserRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUser, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetUserOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

GetUserOutcomeCallable CognitoIdentityProviderClient::GetUserCallable(const GetUserRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetUser, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetUserAsync(const GetUserRequest& request, const GetUserResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetUser, this, request, handler, context, m_executor.get());
}

GetUserAttributeVerificationCodeOutcome CognitoIdentityProviderClient::GetUserAttributeVerificationCode(const GetUserAttributeVerificationCodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUserAttributeVerificationCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUserAttributeVerificationCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetUserAttributeVerificationCodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

GetUserAttributeVerificationCodeOutcomeCallable CognitoIdentityProviderClient::GetUserAttributeVerificationCodeCallable(const GetUserAttributeVerificationCodeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetUserAttributeVerificationCode, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetUserAttributeVerificationCodeAsync(const GetUserAttributeVerificationCodeRequest& request, const GetUserAttributeVerificationCodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetUserAttributeVerificationCode, this, request, handler, context, m_executor.get());
}

GetUserPoolMfaConfigOutcome CognitoIdentityProviderClient::GetUserPoolMfaConfig(const GetUserPoolMfaConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetUserPoolMfaConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetUserPoolMfaConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetUserPoolMfaConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetUserPoolMfaConfigOutcomeCallable CognitoIdentityProviderClient::GetUserPoolMfaConfigCallable(const GetUserPoolMfaConfigRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GetUserPoolMfaConfig, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GetUserPoolMfaConfigAsync(const GetUserPoolMfaConfigRequest& request, const GetUserPoolMfaConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GetUserPoolMfaConfig, this, request, handler, context, m_executor.get());
}

GlobalSignOutOutcome CognitoIdentityProviderClient::GlobalSignOut(const GlobalSignOutRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GlobalSignOut, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GlobalSignOut, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GlobalSignOutOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GlobalSignOutOutcomeCallable CognitoIdentityProviderClient::GlobalSignOutCallable(const GlobalSignOutRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::GlobalSignOut, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::GlobalSignOutAsync(const GlobalSignOutRequest& request, const GlobalSignOutResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::GlobalSignOut, this, request, handler, context, m_executor.get());
}

InitiateAuthOutcome CognitoIdentityProviderClient::InitiateAuth(const InitiateAuthRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, InitiateAuth, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, InitiateAuth, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return InitiateAuthOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

InitiateAuthOutcomeCallable CognitoIdentityProviderClient::InitiateAuthCallable(const InitiateAuthRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::InitiateAuth, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::InitiateAuthAsync(const InitiateAuthRequest& request, const InitiateAuthResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::InitiateAuth, this, request, handler, context, m_executor.get());
}

ListDevicesOutcome CognitoIdentityProviderClient::ListDevices(const ListDevicesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListDevicesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListDevicesOutcomeCallable CognitoIdentityProviderClient::ListDevicesCallable(const ListDevicesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListDevices, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListDevicesAsync(const ListDevicesRequest& request, const ListDevicesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListDevices, this, request, handler, context, m_executor.get());
}

ListGroupsOutcome CognitoIdentityProviderClient::ListGroups(const ListGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListGroupsOutcomeCallable CognitoIdentityProviderClient::ListGroupsCallable(const ListGroupsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListGroups, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListGroupsAsync(const ListGroupsRequest& request, const ListGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListGroups, this, request, handler, context, m_executor.get());
}

ListIdentityProvidersOutcome CognitoIdentityProviderClient::ListIdentityProviders(const ListIdentityProvidersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListIdentityProviders, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListIdentityProviders, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListIdentityProvidersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListIdentityProvidersOutcomeCallable CognitoIdentityProviderClient::ListIdentityProvidersCallable(const ListIdentityProvidersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListIdentityProviders, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListIdentityProvidersAsync(const ListIdentityProvidersRequest& request, const ListIdentityProvidersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListIdentityProviders, this, request, handler, context, m_executor.get());
}

ListResourceServersOutcome CognitoIdentityProviderClient::ListResourceServers(const ListResourceServersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResourceServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResourceServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListResourceServersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListResourceServersOutcomeCallable CognitoIdentityProviderClient::ListResourceServersCallable(const ListResourceServersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListResourceServers, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListResourceServersAsync(const ListResourceServersRequest& request, const ListResourceServersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListResourceServers, this, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome CognitoIdentityProviderClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable CognitoIdentityProviderClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListTagsForResource, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListTagsForResource, this, request, handler, context, m_executor.get());
}

ListUserImportJobsOutcome CognitoIdentityProviderClient::ListUserImportJobs(const ListUserImportJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUserImportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUserImportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListUserImportJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUserImportJobsOutcomeCallable CognitoIdentityProviderClient::ListUserImportJobsCallable(const ListUserImportJobsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListUserImportJobs, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListUserImportJobsAsync(const ListUserImportJobsRequest& request, const ListUserImportJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListUserImportJobs, this, request, handler, context, m_executor.get());
}

ListUserPoolClientsOutcome CognitoIdentityProviderClient::ListUserPoolClients(const ListUserPoolClientsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUserPoolClients, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUserPoolClients, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListUserPoolClientsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUserPoolClientsOutcomeCallable CognitoIdentityProviderClient::ListUserPoolClientsCallable(const ListUserPoolClientsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListUserPoolClients, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListUserPoolClientsAsync(const ListUserPoolClientsRequest& request, const ListUserPoolClientsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListUserPoolClients, this, request, handler, context, m_executor.get());
}

ListUserPoolsOutcome CognitoIdentityProviderClient::ListUserPools(const ListUserPoolsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUserPools, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUserPools, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListUserPoolsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUserPoolsOutcomeCallable CognitoIdentityProviderClient::ListUserPoolsCallable(const ListUserPoolsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListUserPools, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListUserPoolsAsync(const ListUserPoolsRequest& request, const ListUserPoolsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListUserPools, this, request, handler, context, m_executor.get());
}

ListUsersOutcome CognitoIdentityProviderClient::ListUsers(const ListUsersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUsers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUsers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListUsersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUsersOutcomeCallable CognitoIdentityProviderClient::ListUsersCallable(const ListUsersRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListUsers, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListUsersAsync(const ListUsersRequest& request, const ListUsersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListUsers, this, request, handler, context, m_executor.get());
}

ListUsersInGroupOutcome CognitoIdentityProviderClient::ListUsersInGroup(const ListUsersInGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListUsersInGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListUsersInGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListUsersInGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListUsersInGroupOutcomeCallable CognitoIdentityProviderClient::ListUsersInGroupCallable(const ListUsersInGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ListUsersInGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ListUsersInGroupAsync(const ListUsersInGroupRequest& request, const ListUsersInGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ListUsersInGroup, this, request, handler, context, m_executor.get());
}

ResendConfirmationCodeOutcome CognitoIdentityProviderClient::ResendConfirmationCode(const ResendConfirmationCodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResendConfirmationCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResendConfirmationCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResendConfirmationCodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

ResendConfirmationCodeOutcomeCallable CognitoIdentityProviderClient::ResendConfirmationCodeCallable(const ResendConfirmationCodeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::ResendConfirmationCode, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::ResendConfirmationCodeAsync(const ResendConfirmationCodeRequest& request, const ResendConfirmationCodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::ResendConfirmationCode, this, request, handler, context, m_executor.get());
}

RespondToAuthChallengeOutcome CognitoIdentityProviderClient::RespondToAuthChallenge(const RespondToAuthChallengeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RespondToAuthChallenge, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RespondToAuthChallenge, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RespondToAuthChallengeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

RespondToAuthChallengeOutcomeCallable CognitoIdentityProviderClient::RespondToAuthChallengeCallable(const RespondToAuthChallengeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::RespondToAuthChallenge, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::RespondToAuthChallengeAsync(const RespondToAuthChallengeRequest& request, const RespondToAuthChallengeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::RespondToAuthChallenge, this, request, handler, context, m_executor.get());
}

RevokeTokenOutcome CognitoIdentityProviderClient::RevokeToken(const RevokeTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RevokeTokenOutcomeCallable CognitoIdentityProviderClient::RevokeTokenCallable(const RevokeTokenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::RevokeToken, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::RevokeTokenAsync(const RevokeTokenRequest& request, const RevokeTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::RevokeToken, this, request, handler, context, m_executor.get());
}

SetRiskConfigurationOutcome CognitoIdentityProviderClient::SetRiskConfiguration(const SetRiskConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetRiskConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetRiskConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SetRiskConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SetRiskConfigurationOutcomeCallable CognitoIdentityProviderClient::SetRiskConfigurationCallable(const SetRiskConfigurationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SetRiskConfiguration, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SetRiskConfigurationAsync(const SetRiskConfigurationRequest& request, const SetRiskConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SetRiskConfiguration, this, request, handler, context, m_executor.get());
}

SetUICustomizationOutcome CognitoIdentityProviderClient::SetUICustomization(const SetUICustomizationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetUICustomization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetUICustomization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SetUICustomizationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SetUICustomizationOutcomeCallable CognitoIdentityProviderClient::SetUICustomizationCallable(const SetUICustomizationRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SetUICustomization, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SetUICustomizationAsync(const SetUICustomizationRequest& request, const SetUICustomizationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SetUICustomization, this, request, handler, context, m_executor.get());
}

SetUserMFAPreferenceOutcome CognitoIdentityProviderClient::SetUserMFAPreference(const SetUserMFAPreferenceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetUserMFAPreference, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetUserMFAPreference, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SetUserMFAPreferenceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SetUserMFAPreferenceOutcomeCallable CognitoIdentityProviderClient::SetUserMFAPreferenceCallable(const SetUserMFAPreferenceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SetUserMFAPreference, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SetUserMFAPreferenceAsync(const SetUserMFAPreferenceRequest& request, const SetUserMFAPreferenceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SetUserMFAPreference, this, request, handler, context, m_executor.get());
}

SetUserPoolMfaConfigOutcome CognitoIdentityProviderClient::SetUserPoolMfaConfig(const SetUserPoolMfaConfigRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetUserPoolMfaConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetUserPoolMfaConfig, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SetUserPoolMfaConfigOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SetUserPoolMfaConfigOutcomeCallable CognitoIdentityProviderClient::SetUserPoolMfaConfigCallable(const SetUserPoolMfaConfigRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SetUserPoolMfaConfig, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SetUserPoolMfaConfigAsync(const SetUserPoolMfaConfigRequest& request, const SetUserPoolMfaConfigResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SetUserPoolMfaConfig, this, request, handler, context, m_executor.get());
}

SetUserSettingsOutcome CognitoIdentityProviderClient::SetUserSettings(const SetUserSettingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SetUserSettings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SetUserSettings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SetUserSettingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

SetUserSettingsOutcomeCallable CognitoIdentityProviderClient::SetUserSettingsCallable(const SetUserSettingsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SetUserSettings, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SetUserSettingsAsync(const SetUserSettingsRequest& request, const SetUserSettingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SetUserSettings, this, request, handler, context, m_executor.get());
}

SignUpOutcome CognitoIdentityProviderClient::SignUp(const SignUpRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SignUp, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return SignUpOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

SignUpOutcomeCallable CognitoIdentityProviderClient::SignUpCallable(const SignUpRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::SignUp, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::SignUpAsync(const SignUpRequest& request, const SignUpResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::SignUp, this, request, handler, context, m_executor.get());
}

StartUserImportJobOutcome CognitoIdentityProviderClient::StartUserImportJob(const StartUserImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartUserImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartUserImportJobOutcomeCallable CognitoIdentityProviderClient::StartUserImportJobCallable(const StartUserImportJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::StartUserImportJob, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::StartUserImportJobAsync(const StartUserImportJobRequest& request, const StartUserImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::StartUserImportJob, this, request, handler, context, m_executor.get());
}

StopUserImportJobOutcome CognitoIdentityProviderClient::StopUserImportJob(const StopUserImportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopUserImportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StopUserImportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopUserImportJobOutcomeCallable CognitoIdentityProviderClient::StopUserImportJobCallable(const StopUserImportJobRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::StopUserImportJob, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::StopUserImportJobAsync(const StopUserImportJobRequest& request, const StopUserImportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::StopUserImportJob, this, request, handler, context, m_executor.get());
}

TagResourceOutcome CognitoIdentityProviderClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable CognitoIdentityProviderClient::TagResourceCallable(const TagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::TagResource, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::TagResource, this, request, handler, context, m_executor.get());
}

UntagResourceOutcome CognitoIdentityProviderClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable CognitoIdentityProviderClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UntagResource, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UntagResource, this, request, handler, context, m_executor.get());
}

UpdateAuthEventFeedbackOutcome CognitoIdentityProviderClient::UpdateAuthEventFeedback(const UpdateAuthEventFeedbackRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateAuthEventFeedback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateAuthEventFeedback, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateAuthEventFeedbackOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateAuthEventFeedbackOutcomeCallable CognitoIdentityProviderClient::UpdateAuthEventFeedbackCallable(const UpdateAuthEventFeedbackRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateAuthEventFeedback, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateAuthEventFeedbackAsync(const UpdateAuthEventFeedbackRequest& request, const UpdateAuthEventFeedbackResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateAuthEventFeedback, this, request, handler, context, m_executor.get());
}

UpdateDeviceStatusOutcome CognitoIdentityProviderClient::UpdateDeviceStatus(const UpdateDeviceStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDeviceStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDeviceStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateDeviceStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateDeviceStatusOutcomeCallable CognitoIdentityProviderClient::UpdateDeviceStatusCallable(const UpdateDeviceStatusRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateDeviceStatus, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateDeviceStatusAsync(const UpdateDeviceStatusRequest& request, const UpdateDeviceStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateDeviceStatus, this, request, handler, context, m_executor.get());
}

UpdateGroupOutcome CognitoIdentityProviderClient::UpdateGroup(const UpdateGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGroupOutcomeCallable CognitoIdentityProviderClient::UpdateGroupCallable(const UpdateGroupRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateGroup, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateGroupAsync(const UpdateGroupRequest& request, const UpdateGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateGroup, this, request, handler, context, m_executor.get());
}

UpdateIdentityProviderOutcome CognitoIdentityProviderClient::UpdateIdentityProvider(const UpdateIdentityProviderRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateIdentityProvider, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateIdentityProviderOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateIdentityProviderOutcomeCallable CognitoIdentityProviderClient::UpdateIdentityProviderCallable(const UpdateIdentityProviderRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateIdentityProvider, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateIdentityProviderAsync(const UpdateIdentityProviderRequest& request, const UpdateIdentityProviderResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateIdentityProvider, this, request, handler, context, m_executor.get());
}

UpdateResourceServerOutcome CognitoIdentityProviderClient::UpdateResourceServer(const UpdateResourceServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateResourceServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateResourceServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateResourceServerOutcomeCallable CognitoIdentityProviderClient::UpdateResourceServerCallable(const UpdateResourceServerRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateResourceServer, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateResourceServerAsync(const UpdateResourceServerRequest& request, const UpdateResourceServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateResourceServer, this, request, handler, context, m_executor.get());
}

UpdateUserAttributesOutcome CognitoIdentityProviderClient::UpdateUserAttributes(const UpdateUserAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUserAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateUserAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

UpdateUserAttributesOutcomeCallable CognitoIdentityProviderClient::UpdateUserAttributesCallable(const UpdateUserAttributesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateUserAttributes, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateUserAttributesAsync(const UpdateUserAttributesRequest& request, const UpdateUserAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateUserAttributes, this, request, handler, context, m_executor.get());
}

UpdateUserPoolOutcome CognitoIdentityProviderClient::UpdateUserPool(const UpdateUserPoolRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUserPool, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateUserPoolOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateUserPoolOutcomeCallable CognitoIdentityProviderClient::UpdateUserPoolCallable(const UpdateUserPoolRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateUserPool, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateUserPoolAsync(const UpdateUserPoolRequest& request, const UpdateUserPoolResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateUserPool, this, request, handler, context, m_executor.get());
}

UpdateUserPoolClientOutcome CognitoIdentityProviderClient::UpdateUserPoolClient(const UpdateUserPoolClientRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUserPoolClient, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateUserPoolClientOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateUserPoolClientOutcomeCallable CognitoIdentityProviderClient::UpdateUserPoolClientCallable(const UpdateUserPoolClientRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateUserPoolClient, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateUserPoolClientAsync(const UpdateUserPoolClientRequest& request, const UpdateUserPoolClientResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateUserPoolClient, this, request, handler, context, m_executor.get());
}

UpdateUserPoolDomainOutcome CognitoIdentityProviderClient::UpdateUserPoolDomain(const UpdateUserPoolDomainRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateUserPoolDomain, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateUserPoolDomainOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateUserPoolDomainOutcomeCallable CognitoIdentityProviderClient::UpdateUserPoolDomainCallable(const UpdateUserPoolDomainRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::UpdateUserPoolDomain, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::UpdateUserPoolDomainAsync(const UpdateUserPoolDomainRequest& request, const UpdateUserPoolDomainResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::UpdateUserPoolDomain, this, request, handler, context, m_executor.get());
}

VerifySoftwareTokenOutcome CognitoIdentityProviderClient::VerifySoftwareToken(const VerifySoftwareTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, VerifySoftwareToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, VerifySoftwareToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return VerifySoftwareTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

VerifySoftwareTokenOutcomeCallable CognitoIdentityProviderClient::VerifySoftwareTokenCallable(const VerifySoftwareTokenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::VerifySoftwareToken, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::VerifySoftwareTokenAsync(const VerifySoftwareTokenRequest& request, const VerifySoftwareTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::VerifySoftwareToken, this, request, handler, context, m_executor.get());
}

VerifyUserAttributeOutcome CognitoIdentityProviderClient::VerifyUserAttribute(const VerifyUserAttributeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, VerifyUserAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, VerifyUserAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return VerifyUserAttributeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::NULL_SIGNER));
}

VerifyUserAttributeOutcomeCallable CognitoIdentityProviderClient::VerifyUserAttributeCallable(const VerifyUserAttributeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &CognitoIdentityProviderClient::VerifyUserAttribute, this, request, m_executor.get());
}

void CognitoIdentityProviderClient::VerifyUserAttributeAsync(const VerifyUserAttributeRequest& request, const VerifyUserAttributeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&CognitoIdentityProviderClient::VerifyUserAttribute, this, request, handler, context, m_executor.get());
}

