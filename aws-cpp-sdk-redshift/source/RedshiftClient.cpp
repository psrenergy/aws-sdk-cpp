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
#include <aws/core/utils/xml/XmlSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/redshift/RedshiftClient.h>
#include <aws/redshift/RedshiftErrorMarshaller.h>
#include <aws/redshift/RedshiftEndpointProvider.h>
#include <aws/redshift/model/AcceptReservedNodeExchangeRequest.h>
#include <aws/redshift/model/AddPartnerRequest.h>
#include <aws/redshift/model/AssociateDataShareConsumerRequest.h>
#include <aws/redshift/model/AuthorizeClusterSecurityGroupIngressRequest.h>
#include <aws/redshift/model/AuthorizeDataShareRequest.h>
#include <aws/redshift/model/AuthorizeEndpointAccessRequest.h>
#include <aws/redshift/model/AuthorizeSnapshotAccessRequest.h>
#include <aws/redshift/model/BatchDeleteClusterSnapshotsRequest.h>
#include <aws/redshift/model/BatchModifyClusterSnapshotsRequest.h>
#include <aws/redshift/model/CancelResizeRequest.h>
#include <aws/redshift/model/CopyClusterSnapshotRequest.h>
#include <aws/redshift/model/CreateAuthenticationProfileRequest.h>
#include <aws/redshift/model/CreateClusterRequest.h>
#include <aws/redshift/model/CreateClusterParameterGroupRequest.h>
#include <aws/redshift/model/CreateClusterSecurityGroupRequest.h>
#include <aws/redshift/model/CreateClusterSnapshotRequest.h>
#include <aws/redshift/model/CreateClusterSubnetGroupRequest.h>
#include <aws/redshift/model/CreateEndpointAccessRequest.h>
#include <aws/redshift/model/CreateEventSubscriptionRequest.h>
#include <aws/redshift/model/CreateHsmClientCertificateRequest.h>
#include <aws/redshift/model/CreateHsmConfigurationRequest.h>
#include <aws/redshift/model/CreateScheduledActionRequest.h>
#include <aws/redshift/model/CreateSnapshotCopyGrantRequest.h>
#include <aws/redshift/model/CreateSnapshotScheduleRequest.h>
#include <aws/redshift/model/CreateTagsRequest.h>
#include <aws/redshift/model/CreateUsageLimitRequest.h>
#include <aws/redshift/model/DeauthorizeDataShareRequest.h>
#include <aws/redshift/model/DeleteAuthenticationProfileRequest.h>
#include <aws/redshift/model/DeleteClusterRequest.h>
#include <aws/redshift/model/DeleteClusterParameterGroupRequest.h>
#include <aws/redshift/model/DeleteClusterSecurityGroupRequest.h>
#include <aws/redshift/model/DeleteClusterSnapshotRequest.h>
#include <aws/redshift/model/DeleteClusterSubnetGroupRequest.h>
#include <aws/redshift/model/DeleteEndpointAccessRequest.h>
#include <aws/redshift/model/DeleteEventSubscriptionRequest.h>
#include <aws/redshift/model/DeleteHsmClientCertificateRequest.h>
#include <aws/redshift/model/DeleteHsmConfigurationRequest.h>
#include <aws/redshift/model/DeletePartnerRequest.h>
#include <aws/redshift/model/DeleteScheduledActionRequest.h>
#include <aws/redshift/model/DeleteSnapshotCopyGrantRequest.h>
#include <aws/redshift/model/DeleteSnapshotScheduleRequest.h>
#include <aws/redshift/model/DeleteTagsRequest.h>
#include <aws/redshift/model/DeleteUsageLimitRequest.h>
#include <aws/redshift/model/DescribeAccountAttributesRequest.h>
#include <aws/redshift/model/DescribeAuthenticationProfilesRequest.h>
#include <aws/redshift/model/DescribeClusterDbRevisionsRequest.h>
#include <aws/redshift/model/DescribeClusterParameterGroupsRequest.h>
#include <aws/redshift/model/DescribeClusterParametersRequest.h>
#include <aws/redshift/model/DescribeClusterSecurityGroupsRequest.h>
#include <aws/redshift/model/DescribeClusterSnapshotsRequest.h>
#include <aws/redshift/model/DescribeClusterSubnetGroupsRequest.h>
#include <aws/redshift/model/DescribeClusterTracksRequest.h>
#include <aws/redshift/model/DescribeClusterVersionsRequest.h>
#include <aws/redshift/model/DescribeClustersRequest.h>
#include <aws/redshift/model/DescribeDataSharesRequest.h>
#include <aws/redshift/model/DescribeDataSharesForConsumerRequest.h>
#include <aws/redshift/model/DescribeDataSharesForProducerRequest.h>
#include <aws/redshift/model/DescribeDefaultClusterParametersRequest.h>
#include <aws/redshift/model/DescribeEndpointAccessRequest.h>
#include <aws/redshift/model/DescribeEndpointAuthorizationRequest.h>
#include <aws/redshift/model/DescribeEventCategoriesRequest.h>
#include <aws/redshift/model/DescribeEventSubscriptionsRequest.h>
#include <aws/redshift/model/DescribeEventsRequest.h>
#include <aws/redshift/model/DescribeHsmClientCertificatesRequest.h>
#include <aws/redshift/model/DescribeHsmConfigurationsRequest.h>
#include <aws/redshift/model/DescribeLoggingStatusRequest.h>
#include <aws/redshift/model/DescribeNodeConfigurationOptionsRequest.h>
#include <aws/redshift/model/DescribeOrderableClusterOptionsRequest.h>
#include <aws/redshift/model/DescribePartnersRequest.h>
#include <aws/redshift/model/DescribeReservedNodeExchangeStatusRequest.h>
#include <aws/redshift/model/DescribeReservedNodeOfferingsRequest.h>
#include <aws/redshift/model/DescribeReservedNodesRequest.h>
#include <aws/redshift/model/DescribeResizeRequest.h>
#include <aws/redshift/model/DescribeScheduledActionsRequest.h>
#include <aws/redshift/model/DescribeSnapshotCopyGrantsRequest.h>
#include <aws/redshift/model/DescribeSnapshotSchedulesRequest.h>
#include <aws/redshift/model/DescribeStorageRequest.h>
#include <aws/redshift/model/DescribeTableRestoreStatusRequest.h>
#include <aws/redshift/model/DescribeTagsRequest.h>
#include <aws/redshift/model/DescribeUsageLimitsRequest.h>
#include <aws/redshift/model/DisableLoggingRequest.h>
#include <aws/redshift/model/DisableSnapshotCopyRequest.h>
#include <aws/redshift/model/DisassociateDataShareConsumerRequest.h>
#include <aws/redshift/model/EnableLoggingRequest.h>
#include <aws/redshift/model/EnableSnapshotCopyRequest.h>
#include <aws/redshift/model/GetClusterCredentialsRequest.h>
#include <aws/redshift/model/GetClusterCredentialsWithIAMRequest.h>
#include <aws/redshift/model/GetReservedNodeExchangeConfigurationOptionsRequest.h>
#include <aws/redshift/model/GetReservedNodeExchangeOfferingsRequest.h>
#include <aws/redshift/model/ModifyAquaConfigurationRequest.h>
#include <aws/redshift/model/ModifyAuthenticationProfileRequest.h>
#include <aws/redshift/model/ModifyClusterRequest.h>
#include <aws/redshift/model/ModifyClusterDbRevisionRequest.h>
#include <aws/redshift/model/ModifyClusterIamRolesRequest.h>
#include <aws/redshift/model/ModifyClusterMaintenanceRequest.h>
#include <aws/redshift/model/ModifyClusterParameterGroupRequest.h>
#include <aws/redshift/model/ModifyClusterSnapshotRequest.h>
#include <aws/redshift/model/ModifyClusterSnapshotScheduleRequest.h>
#include <aws/redshift/model/ModifyClusterSubnetGroupRequest.h>
#include <aws/redshift/model/ModifyEndpointAccessRequest.h>
#include <aws/redshift/model/ModifyEventSubscriptionRequest.h>
#include <aws/redshift/model/ModifyScheduledActionRequest.h>
#include <aws/redshift/model/ModifySnapshotCopyRetentionPeriodRequest.h>
#include <aws/redshift/model/ModifySnapshotScheduleRequest.h>
#include <aws/redshift/model/ModifyUsageLimitRequest.h>
#include <aws/redshift/model/PauseClusterRequest.h>
#include <aws/redshift/model/PurchaseReservedNodeOfferingRequest.h>
#include <aws/redshift/model/RebootClusterRequest.h>
#include <aws/redshift/model/RejectDataShareRequest.h>
#include <aws/redshift/model/ResetClusterParameterGroupRequest.h>
#include <aws/redshift/model/ResizeClusterRequest.h>
#include <aws/redshift/model/RestoreFromClusterSnapshotRequest.h>
#include <aws/redshift/model/RestoreTableFromClusterSnapshotRequest.h>
#include <aws/redshift/model/ResumeClusterRequest.h>
#include <aws/redshift/model/RevokeClusterSecurityGroupIngressRequest.h>
#include <aws/redshift/model/RevokeEndpointAccessRequest.h>
#include <aws/redshift/model/RevokeSnapshotAccessRequest.h>
#include <aws/redshift/model/RotateEncryptionKeyRequest.h>
#include <aws/redshift/model/UpdatePartnerStatusRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Redshift;
using namespace Aws::Redshift::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Xml;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;


const char* RedshiftClient::SERVICE_NAME = "redshift";
const char* RedshiftClient::ALLOCATION_TAG = "RedshiftClient";

RedshiftClient::RedshiftClient(const Redshift::RedshiftClientConfiguration& clientConfiguration,
                               std::shared_ptr<RedshiftEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RedshiftClient::RedshiftClient(const AWSCredentials& credentials,
                               std::shared_ptr<RedshiftEndpointProviderBase> endpointProvider,
                               const Redshift::RedshiftClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RedshiftClient::RedshiftClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               std::shared_ptr<RedshiftEndpointProviderBase> endpointProvider,
                               const Redshift::RedshiftClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  RedshiftClient::RedshiftClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<RedshiftEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RedshiftClient::RedshiftClient(const AWSCredentials& credentials,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RedshiftEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RedshiftClient::RedshiftClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RedshiftErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RedshiftEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
RedshiftClient::~RedshiftClient()
{
}

std::shared_ptr<RedshiftEndpointProviderBase>& RedshiftClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void RedshiftClient::init(const Redshift::RedshiftClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Redshift");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void RedshiftClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

Aws::String RedshiftClient::ConvertRequestToPresignedUrl(const AmazonSerializableWebServiceRequest& requestToConvert, const char* region) const
{
  if (!m_endpointProvider)
  {
    AWS_LOGSTREAM_ERROR(ALLOCATION_TAG, "Presigned URL generating failed. Endpoint provider is not initialized.");
    return "";
  }
  Aws::Endpoint::EndpointParameters endpointParameters;
  endpointParameters.emplace_back(Aws::Endpoint::EndpointParameter("Region", Aws::String(region)));
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(endpointParameters);
  if (!endpointResolutionOutcome.IsSuccess())
  {
    AWS_LOGSTREAM_ERROR(ALLOCATION_TAG, "Endpoint resolution failed: " << endpointResolutionOutcome.GetError().GetMessage());
    return "";
  }
  Aws::StringStream ss;
  ss << "?" << requestToConvert.SerializePayload();
  endpointResolutionOutcome.GetResult().SetQueryString(ss.str());

  return GeneratePresignedUrl(endpointResolutionOutcome.GetResult().GetURI(), Aws::Http::HttpMethod::HTTP_GET, region, 3600);
}

AcceptReservedNodeExchangeOutcome RedshiftClient::AcceptReservedNodeExchange(const AcceptReservedNodeExchangeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AcceptReservedNodeExchange, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AcceptReservedNodeExchange, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AcceptReservedNodeExchangeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AcceptReservedNodeExchangeOutcomeCallable RedshiftClient::AcceptReservedNodeExchangeCallable(const AcceptReservedNodeExchangeRequest& request) const
{
  std::shared_ptr<AcceptReservedNodeExchangeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AcceptReservedNodeExchangeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AcceptReservedNodeExchange(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AcceptReservedNodeExchangeAsync(const AcceptReservedNodeExchangeRequest& request, const AcceptReservedNodeExchangeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AcceptReservedNodeExchangeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AcceptReservedNodeExchange(*pRequest), context);
    } );
}
AddPartnerOutcome RedshiftClient::AddPartner(const AddPartnerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AddPartner, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AddPartner, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AddPartnerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AddPartnerOutcomeCallable RedshiftClient::AddPartnerCallable(const AddPartnerRequest& request) const
{
  std::shared_ptr<AddPartnerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AddPartnerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AddPartner(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AddPartnerAsync(const AddPartnerRequest& request, const AddPartnerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AddPartnerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AddPartner(*pRequest), context);
    } );
}
AssociateDataShareConsumerOutcome RedshiftClient::AssociateDataShareConsumer(const AssociateDataShareConsumerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateDataShareConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateDataShareConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateDataShareConsumerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AssociateDataShareConsumerOutcomeCallable RedshiftClient::AssociateDataShareConsumerCallable(const AssociateDataShareConsumerRequest& request) const
{
  std::shared_ptr<AssociateDataShareConsumerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AssociateDataShareConsumerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AssociateDataShareConsumer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AssociateDataShareConsumerAsync(const AssociateDataShareConsumerRequest& request, const AssociateDataShareConsumerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AssociateDataShareConsumerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AssociateDataShareConsumer(*pRequest), context);
    } );
}
AuthorizeClusterSecurityGroupIngressOutcome RedshiftClient::AuthorizeClusterSecurityGroupIngress(const AuthorizeClusterSecurityGroupIngressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeClusterSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeClusterSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeClusterSecurityGroupIngressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AuthorizeClusterSecurityGroupIngressOutcomeCallable RedshiftClient::AuthorizeClusterSecurityGroupIngressCallable(const AuthorizeClusterSecurityGroupIngressRequest& request) const
{
  std::shared_ptr<AuthorizeClusterSecurityGroupIngressRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AuthorizeClusterSecurityGroupIngressOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AuthorizeClusterSecurityGroupIngress(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AuthorizeClusterSecurityGroupIngressAsync(const AuthorizeClusterSecurityGroupIngressRequest& request, const AuthorizeClusterSecurityGroupIngressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AuthorizeClusterSecurityGroupIngressRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AuthorizeClusterSecurityGroupIngress(*pRequest), context);
    } );
}
AuthorizeDataShareOutcome RedshiftClient::AuthorizeDataShare(const AuthorizeDataShareRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeDataShareOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AuthorizeDataShareOutcomeCallable RedshiftClient::AuthorizeDataShareCallable(const AuthorizeDataShareRequest& request) const
{
  std::shared_ptr<AuthorizeDataShareRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AuthorizeDataShareOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AuthorizeDataShare(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AuthorizeDataShareAsync(const AuthorizeDataShareRequest& request, const AuthorizeDataShareResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AuthorizeDataShareRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AuthorizeDataShare(*pRequest), context);
    } );
}
AuthorizeEndpointAccessOutcome RedshiftClient::AuthorizeEndpointAccess(const AuthorizeEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AuthorizeEndpointAccessOutcomeCallable RedshiftClient::AuthorizeEndpointAccessCallable(const AuthorizeEndpointAccessRequest& request) const
{
  std::shared_ptr<AuthorizeEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AuthorizeEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AuthorizeEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AuthorizeEndpointAccessAsync(const AuthorizeEndpointAccessRequest& request, const AuthorizeEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AuthorizeEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AuthorizeEndpointAccess(*pRequest), context);
    } );
}
AuthorizeSnapshotAccessOutcome RedshiftClient::AuthorizeSnapshotAccess(const AuthorizeSnapshotAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AuthorizeSnapshotAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AuthorizeSnapshotAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AuthorizeSnapshotAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

AuthorizeSnapshotAccessOutcomeCallable RedshiftClient::AuthorizeSnapshotAccessCallable(const AuthorizeSnapshotAccessRequest& request) const
{
  std::shared_ptr<AuthorizeSnapshotAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AuthorizeSnapshotAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AuthorizeSnapshotAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::AuthorizeSnapshotAccessAsync(const AuthorizeSnapshotAccessRequest& request, const AuthorizeSnapshotAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AuthorizeSnapshotAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AuthorizeSnapshotAccess(*pRequest), context);
    } );
}
BatchDeleteClusterSnapshotsOutcome RedshiftClient::BatchDeleteClusterSnapshots(const BatchDeleteClusterSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchDeleteClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchDeleteClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return BatchDeleteClusterSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

BatchDeleteClusterSnapshotsOutcomeCallable RedshiftClient::BatchDeleteClusterSnapshotsCallable(const BatchDeleteClusterSnapshotsRequest& request) const
{
  std::shared_ptr<BatchDeleteClusterSnapshotsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< BatchDeleteClusterSnapshotsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->BatchDeleteClusterSnapshots(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::BatchDeleteClusterSnapshotsAsync(const BatchDeleteClusterSnapshotsRequest& request, const BatchDeleteClusterSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<BatchDeleteClusterSnapshotsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, BatchDeleteClusterSnapshots(*pRequest), context);
    } );
}
BatchModifyClusterSnapshotsOutcome RedshiftClient::BatchModifyClusterSnapshots(const BatchModifyClusterSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchModifyClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchModifyClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return BatchModifyClusterSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

BatchModifyClusterSnapshotsOutcomeCallable RedshiftClient::BatchModifyClusterSnapshotsCallable(const BatchModifyClusterSnapshotsRequest& request) const
{
  std::shared_ptr<BatchModifyClusterSnapshotsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< BatchModifyClusterSnapshotsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->BatchModifyClusterSnapshots(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::BatchModifyClusterSnapshotsAsync(const BatchModifyClusterSnapshotsRequest& request, const BatchModifyClusterSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<BatchModifyClusterSnapshotsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, BatchModifyClusterSnapshots(*pRequest), context);
    } );
}
CancelResizeOutcome RedshiftClient::CancelResize(const CancelResizeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelResize, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelResize, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CancelResizeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CancelResizeOutcomeCallable RedshiftClient::CancelResizeCallable(const CancelResizeRequest& request) const
{
  std::shared_ptr<CancelResizeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelResizeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelResize(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CancelResizeAsync(const CancelResizeRequest& request, const CancelResizeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelResizeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelResize(*pRequest), context);
    } );
}
CopyClusterSnapshotOutcome RedshiftClient::CopyClusterSnapshot(const CopyClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CopyClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CopyClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CopyClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CopyClusterSnapshotOutcomeCallable RedshiftClient::CopyClusterSnapshotCallable(const CopyClusterSnapshotRequest& request) const
{
  std::shared_ptr<CopyClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CopyClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CopyClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CopyClusterSnapshotAsync(const CopyClusterSnapshotRequest& request, const CopyClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CopyClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CopyClusterSnapshot(*pRequest), context);
    } );
}
CreateAuthenticationProfileOutcome RedshiftClient::CreateAuthenticationProfile(const CreateAuthenticationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateAuthenticationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateAuthenticationProfileOutcomeCallable RedshiftClient::CreateAuthenticationProfileCallable(const CreateAuthenticationProfileRequest& request) const
{
  std::shared_ptr<CreateAuthenticationProfileRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateAuthenticationProfileOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateAuthenticationProfile(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateAuthenticationProfileAsync(const CreateAuthenticationProfileRequest& request, const CreateAuthenticationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateAuthenticationProfileRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateAuthenticationProfile(*pRequest), context);
    } );
}
CreateClusterOutcome RedshiftClient::CreateCluster(const CreateClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateClusterOutcomeCallable RedshiftClient::CreateClusterCallable(const CreateClusterRequest& request) const
{
  std::shared_ptr<CreateClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateClusterAsync(const CreateClusterRequest& request, const CreateClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateCluster(*pRequest), context);
    } );
}
CreateClusterParameterGroupOutcome RedshiftClient::CreateClusterParameterGroup(const CreateClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateClusterParameterGroupOutcomeCallable RedshiftClient::CreateClusterParameterGroupCallable(const CreateClusterParameterGroupRequest& request) const
{
  std::shared_ptr<CreateClusterParameterGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateClusterParameterGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateClusterParameterGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateClusterParameterGroupAsync(const CreateClusterParameterGroupRequest& request, const CreateClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateClusterParameterGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateClusterParameterGroup(*pRequest), context);
    } );
}
CreateClusterSecurityGroupOutcome RedshiftClient::CreateClusterSecurityGroup(const CreateClusterSecurityGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateClusterSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateClusterSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterSecurityGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateClusterSecurityGroupOutcomeCallable RedshiftClient::CreateClusterSecurityGroupCallable(const CreateClusterSecurityGroupRequest& request) const
{
  std::shared_ptr<CreateClusterSecurityGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateClusterSecurityGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateClusterSecurityGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateClusterSecurityGroupAsync(const CreateClusterSecurityGroupRequest& request, const CreateClusterSecurityGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateClusterSecurityGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateClusterSecurityGroup(*pRequest), context);
    } );
}
CreateClusterSnapshotOutcome RedshiftClient::CreateClusterSnapshot(const CreateClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateClusterSnapshotOutcomeCallable RedshiftClient::CreateClusterSnapshotCallable(const CreateClusterSnapshotRequest& request) const
{
  std::shared_ptr<CreateClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateClusterSnapshotAsync(const CreateClusterSnapshotRequest& request, const CreateClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateClusterSnapshot(*pRequest), context);
    } );
}
CreateClusterSubnetGroupOutcome RedshiftClient::CreateClusterSubnetGroup(const CreateClusterSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateClusterSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateClusterSubnetGroupOutcomeCallable RedshiftClient::CreateClusterSubnetGroupCallable(const CreateClusterSubnetGroupRequest& request) const
{
  std::shared_ptr<CreateClusterSubnetGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateClusterSubnetGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateClusterSubnetGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateClusterSubnetGroupAsync(const CreateClusterSubnetGroupRequest& request, const CreateClusterSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateClusterSubnetGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateClusterSubnetGroup(*pRequest), context);
    } );
}
CreateEndpointAccessOutcome RedshiftClient::CreateEndpointAccess(const CreateEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateEndpointAccessOutcomeCallable RedshiftClient::CreateEndpointAccessCallable(const CreateEndpointAccessRequest& request) const
{
  std::shared_ptr<CreateEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateEndpointAccessAsync(const CreateEndpointAccessRequest& request, const CreateEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateEndpointAccess(*pRequest), context);
    } );
}
CreateEventSubscriptionOutcome RedshiftClient::CreateEventSubscription(const CreateEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateEventSubscriptionOutcomeCallable RedshiftClient::CreateEventSubscriptionCallable(const CreateEventSubscriptionRequest& request) const
{
  std::shared_ptr<CreateEventSubscriptionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateEventSubscriptionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateEventSubscription(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateEventSubscriptionAsync(const CreateEventSubscriptionRequest& request, const CreateEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateEventSubscriptionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateEventSubscription(*pRequest), context);
    } );
}
CreateHsmClientCertificateOutcome RedshiftClient::CreateHsmClientCertificate(const CreateHsmClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateHsmClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateHsmClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateHsmClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateHsmClientCertificateOutcomeCallable RedshiftClient::CreateHsmClientCertificateCallable(const CreateHsmClientCertificateRequest& request) const
{
  std::shared_ptr<CreateHsmClientCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateHsmClientCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateHsmClientCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateHsmClientCertificateAsync(const CreateHsmClientCertificateRequest& request, const CreateHsmClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateHsmClientCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateHsmClientCertificate(*pRequest), context);
    } );
}
CreateHsmConfigurationOutcome RedshiftClient::CreateHsmConfiguration(const CreateHsmConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateHsmConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateHsmConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateHsmConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateHsmConfigurationOutcomeCallable RedshiftClient::CreateHsmConfigurationCallable(const CreateHsmConfigurationRequest& request) const
{
  std::shared_ptr<CreateHsmConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateHsmConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateHsmConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateHsmConfigurationAsync(const CreateHsmConfigurationRequest& request, const CreateHsmConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateHsmConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateHsmConfiguration(*pRequest), context);
    } );
}
CreateScheduledActionOutcome RedshiftClient::CreateScheduledAction(const CreateScheduledActionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateScheduledActionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateScheduledActionOutcomeCallable RedshiftClient::CreateScheduledActionCallable(const CreateScheduledActionRequest& request) const
{
  std::shared_ptr<CreateScheduledActionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateScheduledActionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateScheduledAction(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateScheduledActionAsync(const CreateScheduledActionRequest& request, const CreateScheduledActionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateScheduledActionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateScheduledAction(*pRequest), context);
    } );
}
CreateSnapshotCopyGrantOutcome RedshiftClient::CreateSnapshotCopyGrant(const CreateSnapshotCopyGrantRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSnapshotCopyGrant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSnapshotCopyGrant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSnapshotCopyGrantOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateSnapshotCopyGrantOutcomeCallable RedshiftClient::CreateSnapshotCopyGrantCallable(const CreateSnapshotCopyGrantRequest& request) const
{
  std::shared_ptr<CreateSnapshotCopyGrantRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSnapshotCopyGrantOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSnapshotCopyGrant(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateSnapshotCopyGrantAsync(const CreateSnapshotCopyGrantRequest& request, const CreateSnapshotCopyGrantResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSnapshotCopyGrantRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSnapshotCopyGrant(*pRequest), context);
    } );
}
CreateSnapshotScheduleOutcome RedshiftClient::CreateSnapshotSchedule(const CreateSnapshotScheduleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateSnapshotScheduleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateSnapshotScheduleOutcomeCallable RedshiftClient::CreateSnapshotScheduleCallable(const CreateSnapshotScheduleRequest& request) const
{
  std::shared_ptr<CreateSnapshotScheduleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSnapshotScheduleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSnapshotSchedule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateSnapshotScheduleAsync(const CreateSnapshotScheduleRequest& request, const CreateSnapshotScheduleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSnapshotScheduleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSnapshotSchedule(*pRequest), context);
    } );
}
CreateTagsOutcome RedshiftClient::CreateTags(const CreateTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateTagsOutcomeCallable RedshiftClient::CreateTagsCallable(const CreateTagsRequest& request) const
{
  std::shared_ptr<CreateTagsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateTagsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateTags(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateTagsAsync(const CreateTagsRequest& request, const CreateTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateTagsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateTags(*pRequest), context);
    } );
}
CreateUsageLimitOutcome RedshiftClient::CreateUsageLimit(const CreateUsageLimitRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateUsageLimitOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

CreateUsageLimitOutcomeCallable RedshiftClient::CreateUsageLimitCallable(const CreateUsageLimitRequest& request) const
{
  std::shared_ptr<CreateUsageLimitRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateUsageLimitOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateUsageLimit(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::CreateUsageLimitAsync(const CreateUsageLimitRequest& request, const CreateUsageLimitResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateUsageLimitRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateUsageLimit(*pRequest), context);
    } );
}
DeauthorizeDataShareOutcome RedshiftClient::DeauthorizeDataShare(const DeauthorizeDataShareRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeauthorizeDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeauthorizeDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeauthorizeDataShareOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeauthorizeDataShareOutcomeCallable RedshiftClient::DeauthorizeDataShareCallable(const DeauthorizeDataShareRequest& request) const
{
  std::shared_ptr<DeauthorizeDataShareRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeauthorizeDataShareOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeauthorizeDataShare(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeauthorizeDataShareAsync(const DeauthorizeDataShareRequest& request, const DeauthorizeDataShareResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeauthorizeDataShareRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeauthorizeDataShare(*pRequest), context);
    } );
}
DeleteAuthenticationProfileOutcome RedshiftClient::DeleteAuthenticationProfile(const DeleteAuthenticationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteAuthenticationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteAuthenticationProfileOutcomeCallable RedshiftClient::DeleteAuthenticationProfileCallable(const DeleteAuthenticationProfileRequest& request) const
{
  std::shared_ptr<DeleteAuthenticationProfileRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteAuthenticationProfileOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteAuthenticationProfile(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteAuthenticationProfileAsync(const DeleteAuthenticationProfileRequest& request, const DeleteAuthenticationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteAuthenticationProfileRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteAuthenticationProfile(*pRequest), context);
    } );
}
DeleteClusterOutcome RedshiftClient::DeleteCluster(const DeleteClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteClusterOutcomeCallable RedshiftClient::DeleteClusterCallable(const DeleteClusterRequest& request) const
{
  std::shared_ptr<DeleteClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteClusterAsync(const DeleteClusterRequest& request, const DeleteClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteCluster(*pRequest), context);
    } );
}
DeleteClusterParameterGroupOutcome RedshiftClient::DeleteClusterParameterGroup(const DeleteClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteClusterParameterGroupOutcomeCallable RedshiftClient::DeleteClusterParameterGroupCallable(const DeleteClusterParameterGroupRequest& request) const
{
  std::shared_ptr<DeleteClusterParameterGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteClusterParameterGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteClusterParameterGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteClusterParameterGroupAsync(const DeleteClusterParameterGroupRequest& request, const DeleteClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteClusterParameterGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteClusterParameterGroup(*pRequest), context);
    } );
}
DeleteClusterSecurityGroupOutcome RedshiftClient::DeleteClusterSecurityGroup(const DeleteClusterSecurityGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClusterSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClusterSecurityGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterSecurityGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteClusterSecurityGroupOutcomeCallable RedshiftClient::DeleteClusterSecurityGroupCallable(const DeleteClusterSecurityGroupRequest& request) const
{
  std::shared_ptr<DeleteClusterSecurityGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteClusterSecurityGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteClusterSecurityGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteClusterSecurityGroupAsync(const DeleteClusterSecurityGroupRequest& request, const DeleteClusterSecurityGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteClusterSecurityGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteClusterSecurityGroup(*pRequest), context);
    } );
}
DeleteClusterSnapshotOutcome RedshiftClient::DeleteClusterSnapshot(const DeleteClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteClusterSnapshotOutcomeCallable RedshiftClient::DeleteClusterSnapshotCallable(const DeleteClusterSnapshotRequest& request) const
{
  std::shared_ptr<DeleteClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteClusterSnapshotAsync(const DeleteClusterSnapshotRequest& request, const DeleteClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteClusterSnapshot(*pRequest), context);
    } );
}
DeleteClusterSubnetGroupOutcome RedshiftClient::DeleteClusterSubnetGroup(const DeleteClusterSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteClusterSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteClusterSubnetGroupOutcomeCallable RedshiftClient::DeleteClusterSubnetGroupCallable(const DeleteClusterSubnetGroupRequest& request) const
{
  std::shared_ptr<DeleteClusterSubnetGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteClusterSubnetGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteClusterSubnetGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteClusterSubnetGroupAsync(const DeleteClusterSubnetGroupRequest& request, const DeleteClusterSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteClusterSubnetGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteClusterSubnetGroup(*pRequest), context);
    } );
}
DeleteEndpointAccessOutcome RedshiftClient::DeleteEndpointAccess(const DeleteEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteEndpointAccessOutcomeCallable RedshiftClient::DeleteEndpointAccessCallable(const DeleteEndpointAccessRequest& request) const
{
  std::shared_ptr<DeleteEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteEndpointAccessAsync(const DeleteEndpointAccessRequest& request, const DeleteEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteEndpointAccess(*pRequest), context);
    } );
}
DeleteEventSubscriptionOutcome RedshiftClient::DeleteEventSubscription(const DeleteEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteEventSubscriptionOutcomeCallable RedshiftClient::DeleteEventSubscriptionCallable(const DeleteEventSubscriptionRequest& request) const
{
  std::shared_ptr<DeleteEventSubscriptionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteEventSubscriptionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteEventSubscription(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteEventSubscriptionAsync(const DeleteEventSubscriptionRequest& request, const DeleteEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteEventSubscriptionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteEventSubscription(*pRequest), context);
    } );
}
DeleteHsmClientCertificateOutcome RedshiftClient::DeleteHsmClientCertificate(const DeleteHsmClientCertificateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteHsmClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteHsmClientCertificate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteHsmClientCertificateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteHsmClientCertificateOutcomeCallable RedshiftClient::DeleteHsmClientCertificateCallable(const DeleteHsmClientCertificateRequest& request) const
{
  std::shared_ptr<DeleteHsmClientCertificateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteHsmClientCertificateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteHsmClientCertificate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteHsmClientCertificateAsync(const DeleteHsmClientCertificateRequest& request, const DeleteHsmClientCertificateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteHsmClientCertificateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteHsmClientCertificate(*pRequest), context);
    } );
}
DeleteHsmConfigurationOutcome RedshiftClient::DeleteHsmConfiguration(const DeleteHsmConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteHsmConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteHsmConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteHsmConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteHsmConfigurationOutcomeCallable RedshiftClient::DeleteHsmConfigurationCallable(const DeleteHsmConfigurationRequest& request) const
{
  std::shared_ptr<DeleteHsmConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteHsmConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteHsmConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteHsmConfigurationAsync(const DeleteHsmConfigurationRequest& request, const DeleteHsmConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteHsmConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteHsmConfiguration(*pRequest), context);
    } );
}
DeletePartnerOutcome RedshiftClient::DeletePartner(const DeletePartnerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePartner, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePartner, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeletePartnerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeletePartnerOutcomeCallable RedshiftClient::DeletePartnerCallable(const DeletePartnerRequest& request) const
{
  std::shared_ptr<DeletePartnerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeletePartnerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeletePartner(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeletePartnerAsync(const DeletePartnerRequest& request, const DeletePartnerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeletePartnerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeletePartner(*pRequest), context);
    } );
}
DeleteScheduledActionOutcome RedshiftClient::DeleteScheduledAction(const DeleteScheduledActionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteScheduledActionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteScheduledActionOutcomeCallable RedshiftClient::DeleteScheduledActionCallable(const DeleteScheduledActionRequest& request) const
{
  std::shared_ptr<DeleteScheduledActionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteScheduledActionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteScheduledAction(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteScheduledActionAsync(const DeleteScheduledActionRequest& request, const DeleteScheduledActionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteScheduledActionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteScheduledAction(*pRequest), context);
    } );
}
DeleteSnapshotCopyGrantOutcome RedshiftClient::DeleteSnapshotCopyGrant(const DeleteSnapshotCopyGrantRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSnapshotCopyGrant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSnapshotCopyGrant, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSnapshotCopyGrantOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteSnapshotCopyGrantOutcomeCallable RedshiftClient::DeleteSnapshotCopyGrantCallable(const DeleteSnapshotCopyGrantRequest& request) const
{
  std::shared_ptr<DeleteSnapshotCopyGrantRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSnapshotCopyGrantOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSnapshotCopyGrant(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteSnapshotCopyGrantAsync(const DeleteSnapshotCopyGrantRequest& request, const DeleteSnapshotCopyGrantResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSnapshotCopyGrantRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSnapshotCopyGrant(*pRequest), context);
    } );
}
DeleteSnapshotScheduleOutcome RedshiftClient::DeleteSnapshotSchedule(const DeleteSnapshotScheduleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteSnapshotScheduleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteSnapshotScheduleOutcomeCallable RedshiftClient::DeleteSnapshotScheduleCallable(const DeleteSnapshotScheduleRequest& request) const
{
  std::shared_ptr<DeleteSnapshotScheduleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSnapshotScheduleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSnapshotSchedule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteSnapshotScheduleAsync(const DeleteSnapshotScheduleRequest& request, const DeleteSnapshotScheduleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSnapshotScheduleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSnapshotSchedule(*pRequest), context);
    } );
}
DeleteTagsOutcome RedshiftClient::DeleteTags(const DeleteTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteTagsOutcomeCallable RedshiftClient::DeleteTagsCallable(const DeleteTagsRequest& request) const
{
  std::shared_ptr<DeleteTagsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteTagsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteTags(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteTagsAsync(const DeleteTagsRequest& request, const DeleteTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteTagsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteTags(*pRequest), context);
    } );
}
DeleteUsageLimitOutcome RedshiftClient::DeleteUsageLimit(const DeleteUsageLimitRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteUsageLimitOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DeleteUsageLimitOutcomeCallable RedshiftClient::DeleteUsageLimitCallable(const DeleteUsageLimitRequest& request) const
{
  std::shared_ptr<DeleteUsageLimitRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteUsageLimitOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteUsageLimit(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DeleteUsageLimitAsync(const DeleteUsageLimitRequest& request, const DeleteUsageLimitResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteUsageLimitRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteUsageLimit(*pRequest), context);
    } );
}
DescribeAccountAttributesOutcome RedshiftClient::DescribeAccountAttributes(const DescribeAccountAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeAccountAttributesOutcomeCallable RedshiftClient::DescribeAccountAttributesCallable(const DescribeAccountAttributesRequest& request) const
{
  std::shared_ptr<DescribeAccountAttributesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeAccountAttributesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeAccountAttributes(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeAccountAttributesAsync(const DescribeAccountAttributesRequest& request, const DescribeAccountAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeAccountAttributesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeAccountAttributes(*pRequest), context);
    } );
}
DescribeAuthenticationProfilesOutcome RedshiftClient::DescribeAuthenticationProfiles(const DescribeAuthenticationProfilesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAuthenticationProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAuthenticationProfiles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAuthenticationProfilesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeAuthenticationProfilesOutcomeCallable RedshiftClient::DescribeAuthenticationProfilesCallable(const DescribeAuthenticationProfilesRequest& request) const
{
  std::shared_ptr<DescribeAuthenticationProfilesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeAuthenticationProfilesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeAuthenticationProfiles(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeAuthenticationProfilesAsync(const DescribeAuthenticationProfilesRequest& request, const DescribeAuthenticationProfilesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeAuthenticationProfilesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeAuthenticationProfiles(*pRequest), context);
    } );
}
DescribeClusterDbRevisionsOutcome RedshiftClient::DescribeClusterDbRevisions(const DescribeClusterDbRevisionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterDbRevisions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterDbRevisions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterDbRevisionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterDbRevisionsOutcomeCallable RedshiftClient::DescribeClusterDbRevisionsCallable(const DescribeClusterDbRevisionsRequest& request) const
{
  std::shared_ptr<DescribeClusterDbRevisionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterDbRevisionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterDbRevisions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterDbRevisionsAsync(const DescribeClusterDbRevisionsRequest& request, const DescribeClusterDbRevisionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterDbRevisionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterDbRevisions(*pRequest), context);
    } );
}
DescribeClusterParameterGroupsOutcome RedshiftClient::DescribeClusterParameterGroups(const DescribeClusterParameterGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterParameterGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterParameterGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterParameterGroupsOutcomeCallable RedshiftClient::DescribeClusterParameterGroupsCallable(const DescribeClusterParameterGroupsRequest& request) const
{
  std::shared_ptr<DescribeClusterParameterGroupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterParameterGroupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterParameterGroups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterParameterGroupsAsync(const DescribeClusterParameterGroupsRequest& request, const DescribeClusterParameterGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterParameterGroupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterParameterGroups(*pRequest), context);
    } );
}
DescribeClusterParametersOutcome RedshiftClient::DescribeClusterParameters(const DescribeClusterParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterParametersOutcomeCallable RedshiftClient::DescribeClusterParametersCallable(const DescribeClusterParametersRequest& request) const
{
  std::shared_ptr<DescribeClusterParametersRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterParametersOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterParameters(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterParametersAsync(const DescribeClusterParametersRequest& request, const DescribeClusterParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterParametersRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterParameters(*pRequest), context);
    } );
}
DescribeClusterSecurityGroupsOutcome RedshiftClient::DescribeClusterSecurityGroups(const DescribeClusterSecurityGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterSecurityGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterSecurityGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterSecurityGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterSecurityGroupsOutcomeCallable RedshiftClient::DescribeClusterSecurityGroupsCallable(const DescribeClusterSecurityGroupsRequest& request) const
{
  std::shared_ptr<DescribeClusterSecurityGroupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterSecurityGroupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterSecurityGroups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterSecurityGroupsAsync(const DescribeClusterSecurityGroupsRequest& request, const DescribeClusterSecurityGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterSecurityGroupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterSecurityGroups(*pRequest), context);
    } );
}
DescribeClusterSnapshotsOutcome RedshiftClient::DescribeClusterSnapshots(const DescribeClusterSnapshotsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterSnapshots, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterSnapshotsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterSnapshotsOutcomeCallable RedshiftClient::DescribeClusterSnapshotsCallable(const DescribeClusterSnapshotsRequest& request) const
{
  std::shared_ptr<DescribeClusterSnapshotsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterSnapshotsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterSnapshots(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterSnapshotsAsync(const DescribeClusterSnapshotsRequest& request, const DescribeClusterSnapshotsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterSnapshotsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterSnapshots(*pRequest), context);
    } );
}
DescribeClusterSubnetGroupsOutcome RedshiftClient::DescribeClusterSubnetGroups(const DescribeClusterSubnetGroupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterSubnetGroups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterSubnetGroupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterSubnetGroupsOutcomeCallable RedshiftClient::DescribeClusterSubnetGroupsCallable(const DescribeClusterSubnetGroupsRequest& request) const
{
  std::shared_ptr<DescribeClusterSubnetGroupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterSubnetGroupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterSubnetGroups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterSubnetGroupsAsync(const DescribeClusterSubnetGroupsRequest& request, const DescribeClusterSubnetGroupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterSubnetGroupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterSubnetGroups(*pRequest), context);
    } );
}
DescribeClusterTracksOutcome RedshiftClient::DescribeClusterTracks(const DescribeClusterTracksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterTracks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterTracks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterTracksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterTracksOutcomeCallable RedshiftClient::DescribeClusterTracksCallable(const DescribeClusterTracksRequest& request) const
{
  std::shared_ptr<DescribeClusterTracksRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterTracksOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterTracks(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterTracksAsync(const DescribeClusterTracksRequest& request, const DescribeClusterTracksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterTracksRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterTracks(*pRequest), context);
    } );
}
DescribeClusterVersionsOutcome RedshiftClient::DescribeClusterVersions(const DescribeClusterVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusterVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusterVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClusterVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClusterVersionsOutcomeCallable RedshiftClient::DescribeClusterVersionsCallable(const DescribeClusterVersionsRequest& request) const
{
  std::shared_ptr<DescribeClusterVersionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClusterVersionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusterVersions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClusterVersionsAsync(const DescribeClusterVersionsRequest& request, const DescribeClusterVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClusterVersionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusterVersions(*pRequest), context);
    } );
}
DescribeClustersOutcome RedshiftClient::DescribeClusters(const DescribeClustersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeClusters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeClustersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeClustersOutcomeCallable RedshiftClient::DescribeClustersCallable(const DescribeClustersRequest& request) const
{
  std::shared_ptr<DescribeClustersRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeClustersOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeClusters(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeClustersAsync(const DescribeClustersRequest& request, const DescribeClustersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeClustersRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeClusters(*pRequest), context);
    } );
}
DescribeDataSharesOutcome RedshiftClient::DescribeDataShares(const DescribeDataSharesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDataShares, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDataShares, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDataSharesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDataSharesOutcomeCallable RedshiftClient::DescribeDataSharesCallable(const DescribeDataSharesRequest& request) const
{
  std::shared_ptr<DescribeDataSharesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeDataSharesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeDataShares(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeDataSharesAsync(const DescribeDataSharesRequest& request, const DescribeDataSharesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeDataSharesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeDataShares(*pRequest), context);
    } );
}
DescribeDataSharesForConsumerOutcome RedshiftClient::DescribeDataSharesForConsumer(const DescribeDataSharesForConsumerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDataSharesForConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDataSharesForConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDataSharesForConsumerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDataSharesForConsumerOutcomeCallable RedshiftClient::DescribeDataSharesForConsumerCallable(const DescribeDataSharesForConsumerRequest& request) const
{
  std::shared_ptr<DescribeDataSharesForConsumerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeDataSharesForConsumerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeDataSharesForConsumer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeDataSharesForConsumerAsync(const DescribeDataSharesForConsumerRequest& request, const DescribeDataSharesForConsumerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeDataSharesForConsumerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeDataSharesForConsumer(*pRequest), context);
    } );
}
DescribeDataSharesForProducerOutcome RedshiftClient::DescribeDataSharesForProducer(const DescribeDataSharesForProducerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDataSharesForProducer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDataSharesForProducer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDataSharesForProducerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDataSharesForProducerOutcomeCallable RedshiftClient::DescribeDataSharesForProducerCallable(const DescribeDataSharesForProducerRequest& request) const
{
  std::shared_ptr<DescribeDataSharesForProducerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeDataSharesForProducerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeDataSharesForProducer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeDataSharesForProducerAsync(const DescribeDataSharesForProducerRequest& request, const DescribeDataSharesForProducerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeDataSharesForProducerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeDataSharesForProducer(*pRequest), context);
    } );
}
DescribeDefaultClusterParametersOutcome RedshiftClient::DescribeDefaultClusterParameters(const DescribeDefaultClusterParametersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDefaultClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDefaultClusterParameters, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeDefaultClusterParametersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeDefaultClusterParametersOutcomeCallable RedshiftClient::DescribeDefaultClusterParametersCallable(const DescribeDefaultClusterParametersRequest& request) const
{
  std::shared_ptr<DescribeDefaultClusterParametersRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeDefaultClusterParametersOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeDefaultClusterParameters(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeDefaultClusterParametersAsync(const DescribeDefaultClusterParametersRequest& request, const DescribeDefaultClusterParametersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeDefaultClusterParametersRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeDefaultClusterParameters(*pRequest), context);
    } );
}
DescribeEndpointAccessOutcome RedshiftClient::DescribeEndpointAccess(const DescribeEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEndpointAccessOutcomeCallable RedshiftClient::DescribeEndpointAccessCallable(const DescribeEndpointAccessRequest& request) const
{
  std::shared_ptr<DescribeEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeEndpointAccessAsync(const DescribeEndpointAccessRequest& request, const DescribeEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEndpointAccess(*pRequest), context);
    } );
}
DescribeEndpointAuthorizationOutcome RedshiftClient::DescribeEndpointAuthorization(const DescribeEndpointAuthorizationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEndpointAuthorization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEndpointAuthorization, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEndpointAuthorizationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEndpointAuthorizationOutcomeCallable RedshiftClient::DescribeEndpointAuthorizationCallable(const DescribeEndpointAuthorizationRequest& request) const
{
  std::shared_ptr<DescribeEndpointAuthorizationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEndpointAuthorizationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEndpointAuthorization(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeEndpointAuthorizationAsync(const DescribeEndpointAuthorizationRequest& request, const DescribeEndpointAuthorizationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEndpointAuthorizationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEndpointAuthorization(*pRequest), context);
    } );
}
DescribeEventCategoriesOutcome RedshiftClient::DescribeEventCategories(const DescribeEventCategoriesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEventCategories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEventCategories, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventCategoriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventCategoriesOutcomeCallable RedshiftClient::DescribeEventCategoriesCallable(const DescribeEventCategoriesRequest& request) const
{
  std::shared_ptr<DescribeEventCategoriesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEventCategoriesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEventCategories(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeEventCategoriesAsync(const DescribeEventCategoriesRequest& request, const DescribeEventCategoriesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEventCategoriesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEventCategories(*pRequest), context);
    } );
}
DescribeEventSubscriptionsOutcome RedshiftClient::DescribeEventSubscriptions(const DescribeEventSubscriptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEventSubscriptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEventSubscriptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventSubscriptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventSubscriptionsOutcomeCallable RedshiftClient::DescribeEventSubscriptionsCallable(const DescribeEventSubscriptionsRequest& request) const
{
  std::shared_ptr<DescribeEventSubscriptionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEventSubscriptionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEventSubscriptions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeEventSubscriptionsAsync(const DescribeEventSubscriptionsRequest& request, const DescribeEventSubscriptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEventSubscriptionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEventSubscriptions(*pRequest), context);
    } );
}
DescribeEventsOutcome RedshiftClient::DescribeEvents(const DescribeEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeEventsOutcomeCallable RedshiftClient::DescribeEventsCallable(const DescribeEventsRequest& request) const
{
  std::shared_ptr<DescribeEventsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEventsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEvents(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeEventsAsync(const DescribeEventsRequest& request, const DescribeEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEventsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEvents(*pRequest), context);
    } );
}
DescribeHsmClientCertificatesOutcome RedshiftClient::DescribeHsmClientCertificates(const DescribeHsmClientCertificatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeHsmClientCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeHsmClientCertificates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeHsmClientCertificatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeHsmClientCertificatesOutcomeCallable RedshiftClient::DescribeHsmClientCertificatesCallable(const DescribeHsmClientCertificatesRequest& request) const
{
  std::shared_ptr<DescribeHsmClientCertificatesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeHsmClientCertificatesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeHsmClientCertificates(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeHsmClientCertificatesAsync(const DescribeHsmClientCertificatesRequest& request, const DescribeHsmClientCertificatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeHsmClientCertificatesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeHsmClientCertificates(*pRequest), context);
    } );
}
DescribeHsmConfigurationsOutcome RedshiftClient::DescribeHsmConfigurations(const DescribeHsmConfigurationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeHsmConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeHsmConfigurations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeHsmConfigurationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeHsmConfigurationsOutcomeCallable RedshiftClient::DescribeHsmConfigurationsCallable(const DescribeHsmConfigurationsRequest& request) const
{
  std::shared_ptr<DescribeHsmConfigurationsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeHsmConfigurationsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeHsmConfigurations(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeHsmConfigurationsAsync(const DescribeHsmConfigurationsRequest& request, const DescribeHsmConfigurationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeHsmConfigurationsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeHsmConfigurations(*pRequest), context);
    } );
}
DescribeLoggingStatusOutcome RedshiftClient::DescribeLoggingStatus(const DescribeLoggingStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeLoggingStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeLoggingStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeLoggingStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeLoggingStatusOutcomeCallable RedshiftClient::DescribeLoggingStatusCallable(const DescribeLoggingStatusRequest& request) const
{
  std::shared_ptr<DescribeLoggingStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeLoggingStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeLoggingStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeLoggingStatusAsync(const DescribeLoggingStatusRequest& request, const DescribeLoggingStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeLoggingStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeLoggingStatus(*pRequest), context);
    } );
}
DescribeNodeConfigurationOptionsOutcome RedshiftClient::DescribeNodeConfigurationOptions(const DescribeNodeConfigurationOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeNodeConfigurationOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeNodeConfigurationOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeNodeConfigurationOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeNodeConfigurationOptionsOutcomeCallable RedshiftClient::DescribeNodeConfigurationOptionsCallable(const DescribeNodeConfigurationOptionsRequest& request) const
{
  std::shared_ptr<DescribeNodeConfigurationOptionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeNodeConfigurationOptionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeNodeConfigurationOptions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeNodeConfigurationOptionsAsync(const DescribeNodeConfigurationOptionsRequest& request, const DescribeNodeConfigurationOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeNodeConfigurationOptionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeNodeConfigurationOptions(*pRequest), context);
    } );
}
DescribeOrderableClusterOptionsOutcome RedshiftClient::DescribeOrderableClusterOptions(const DescribeOrderableClusterOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeOrderableClusterOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeOrderableClusterOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeOrderableClusterOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeOrderableClusterOptionsOutcomeCallable RedshiftClient::DescribeOrderableClusterOptionsCallable(const DescribeOrderableClusterOptionsRequest& request) const
{
  std::shared_ptr<DescribeOrderableClusterOptionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeOrderableClusterOptionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeOrderableClusterOptions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeOrderableClusterOptionsAsync(const DescribeOrderableClusterOptionsRequest& request, const DescribeOrderableClusterOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeOrderableClusterOptionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeOrderableClusterOptions(*pRequest), context);
    } );
}
DescribePartnersOutcome RedshiftClient::DescribePartners(const DescribePartnersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribePartners, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribePartners, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribePartnersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribePartnersOutcomeCallable RedshiftClient::DescribePartnersCallable(const DescribePartnersRequest& request) const
{
  std::shared_ptr<DescribePartnersRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribePartnersOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribePartners(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribePartnersAsync(const DescribePartnersRequest& request, const DescribePartnersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribePartnersRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribePartners(*pRequest), context);
    } );
}
DescribeReservedNodeExchangeStatusOutcome RedshiftClient::DescribeReservedNodeExchangeStatus(const DescribeReservedNodeExchangeStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReservedNodeExchangeStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReservedNodeExchangeStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReservedNodeExchangeStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeReservedNodeExchangeStatusOutcomeCallable RedshiftClient::DescribeReservedNodeExchangeStatusCallable(const DescribeReservedNodeExchangeStatusRequest& request) const
{
  std::shared_ptr<DescribeReservedNodeExchangeStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeReservedNodeExchangeStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeReservedNodeExchangeStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeReservedNodeExchangeStatusAsync(const DescribeReservedNodeExchangeStatusRequest& request, const DescribeReservedNodeExchangeStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeReservedNodeExchangeStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeReservedNodeExchangeStatus(*pRequest), context);
    } );
}
DescribeReservedNodeOfferingsOutcome RedshiftClient::DescribeReservedNodeOfferings(const DescribeReservedNodeOfferingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReservedNodeOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReservedNodeOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReservedNodeOfferingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeReservedNodeOfferingsOutcomeCallable RedshiftClient::DescribeReservedNodeOfferingsCallable(const DescribeReservedNodeOfferingsRequest& request) const
{
  std::shared_ptr<DescribeReservedNodeOfferingsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeReservedNodeOfferingsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeReservedNodeOfferings(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeReservedNodeOfferingsAsync(const DescribeReservedNodeOfferingsRequest& request, const DescribeReservedNodeOfferingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeReservedNodeOfferingsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeReservedNodeOfferings(*pRequest), context);
    } );
}
DescribeReservedNodesOutcome RedshiftClient::DescribeReservedNodes(const DescribeReservedNodesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeReservedNodes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeReservedNodes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeReservedNodesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeReservedNodesOutcomeCallable RedshiftClient::DescribeReservedNodesCallable(const DescribeReservedNodesRequest& request) const
{
  std::shared_ptr<DescribeReservedNodesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeReservedNodesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeReservedNodes(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeReservedNodesAsync(const DescribeReservedNodesRequest& request, const DescribeReservedNodesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeReservedNodesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeReservedNodes(*pRequest), context);
    } );
}
DescribeResizeOutcome RedshiftClient::DescribeResize(const DescribeResizeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeResize, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeResize, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeResizeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeResizeOutcomeCallable RedshiftClient::DescribeResizeCallable(const DescribeResizeRequest& request) const
{
  std::shared_ptr<DescribeResizeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeResizeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeResize(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeResizeAsync(const DescribeResizeRequest& request, const DescribeResizeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeResizeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeResize(*pRequest), context);
    } );
}
DescribeScheduledActionsOutcome RedshiftClient::DescribeScheduledActions(const DescribeScheduledActionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeScheduledActions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeScheduledActions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeScheduledActionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeScheduledActionsOutcomeCallable RedshiftClient::DescribeScheduledActionsCallable(const DescribeScheduledActionsRequest& request) const
{
  std::shared_ptr<DescribeScheduledActionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeScheduledActionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeScheduledActions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeScheduledActionsAsync(const DescribeScheduledActionsRequest& request, const DescribeScheduledActionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeScheduledActionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeScheduledActions(*pRequest), context);
    } );
}
DescribeSnapshotCopyGrantsOutcome RedshiftClient::DescribeSnapshotCopyGrants(const DescribeSnapshotCopyGrantsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSnapshotCopyGrants, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSnapshotCopyGrants, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeSnapshotCopyGrantsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeSnapshotCopyGrantsOutcomeCallable RedshiftClient::DescribeSnapshotCopyGrantsCallable(const DescribeSnapshotCopyGrantsRequest& request) const
{
  std::shared_ptr<DescribeSnapshotCopyGrantsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSnapshotCopyGrantsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSnapshotCopyGrants(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeSnapshotCopyGrantsAsync(const DescribeSnapshotCopyGrantsRequest& request, const DescribeSnapshotCopyGrantsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSnapshotCopyGrantsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSnapshotCopyGrants(*pRequest), context);
    } );
}
DescribeSnapshotSchedulesOutcome RedshiftClient::DescribeSnapshotSchedules(const DescribeSnapshotSchedulesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSnapshotSchedules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSnapshotSchedules, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeSnapshotSchedulesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeSnapshotSchedulesOutcomeCallable RedshiftClient::DescribeSnapshotSchedulesCallable(const DescribeSnapshotSchedulesRequest& request) const
{
  std::shared_ptr<DescribeSnapshotSchedulesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSnapshotSchedulesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSnapshotSchedules(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeSnapshotSchedulesAsync(const DescribeSnapshotSchedulesRequest& request, const DescribeSnapshotSchedulesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSnapshotSchedulesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSnapshotSchedules(*pRequest), context);
    } );
}
DescribeStorageOutcome RedshiftClient::DescribeStorage(const DescribeStorageRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeStorage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeStorage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeStorageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeStorageOutcomeCallable RedshiftClient::DescribeStorageCallable(const DescribeStorageRequest& request) const
{
  std::shared_ptr<DescribeStorageRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeStorageOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeStorage(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeStorageAsync(const DescribeStorageRequest& request, const DescribeStorageResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeStorageRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeStorage(*pRequest), context);
    } );
}
DescribeTableRestoreStatusOutcome RedshiftClient::DescribeTableRestoreStatus(const DescribeTableRestoreStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeTableRestoreStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeTableRestoreStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeTableRestoreStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeTableRestoreStatusOutcomeCallable RedshiftClient::DescribeTableRestoreStatusCallable(const DescribeTableRestoreStatusRequest& request) const
{
  std::shared_ptr<DescribeTableRestoreStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeTableRestoreStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeTableRestoreStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeTableRestoreStatusAsync(const DescribeTableRestoreStatusRequest& request, const DescribeTableRestoreStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeTableRestoreStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeTableRestoreStatus(*pRequest), context);
    } );
}
DescribeTagsOutcome RedshiftClient::DescribeTags(const DescribeTagsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeTags, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeTagsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeTagsOutcomeCallable RedshiftClient::DescribeTagsCallable(const DescribeTagsRequest& request) const
{
  std::shared_ptr<DescribeTagsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeTagsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeTags(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeTagsAsync(const DescribeTagsRequest& request, const DescribeTagsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeTagsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeTags(*pRequest), context);
    } );
}
DescribeUsageLimitsOutcome RedshiftClient::DescribeUsageLimits(const DescribeUsageLimitsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeUsageLimits, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeUsageLimits, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeUsageLimitsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DescribeUsageLimitsOutcomeCallable RedshiftClient::DescribeUsageLimitsCallable(const DescribeUsageLimitsRequest& request) const
{
  std::shared_ptr<DescribeUsageLimitsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeUsageLimitsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeUsageLimits(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DescribeUsageLimitsAsync(const DescribeUsageLimitsRequest& request, const DescribeUsageLimitsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeUsageLimitsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeUsageLimits(*pRequest), context);
    } );
}
DisableLoggingOutcome RedshiftClient::DisableLogging(const DisableLoggingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisableLogging, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisableLogging, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisableLoggingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DisableLoggingOutcomeCallable RedshiftClient::DisableLoggingCallable(const DisableLoggingRequest& request) const
{
  std::shared_ptr<DisableLoggingRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DisableLoggingOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DisableLogging(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DisableLoggingAsync(const DisableLoggingRequest& request, const DisableLoggingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DisableLoggingRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DisableLogging(*pRequest), context);
    } );
}
DisableSnapshotCopyOutcome RedshiftClient::DisableSnapshotCopy(const DisableSnapshotCopyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisableSnapshotCopy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisableSnapshotCopy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisableSnapshotCopyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DisableSnapshotCopyOutcomeCallable RedshiftClient::DisableSnapshotCopyCallable(const DisableSnapshotCopyRequest& request) const
{
  std::shared_ptr<DisableSnapshotCopyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DisableSnapshotCopyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DisableSnapshotCopy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DisableSnapshotCopyAsync(const DisableSnapshotCopyRequest& request, const DisableSnapshotCopyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DisableSnapshotCopyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DisableSnapshotCopy(*pRequest), context);
    } );
}
DisassociateDataShareConsumerOutcome RedshiftClient::DisassociateDataShareConsumer(const DisassociateDataShareConsumerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateDataShareConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateDataShareConsumer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateDataShareConsumerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

DisassociateDataShareConsumerOutcomeCallable RedshiftClient::DisassociateDataShareConsumerCallable(const DisassociateDataShareConsumerRequest& request) const
{
  std::shared_ptr<DisassociateDataShareConsumerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DisassociateDataShareConsumerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DisassociateDataShareConsumer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::DisassociateDataShareConsumerAsync(const DisassociateDataShareConsumerRequest& request, const DisassociateDataShareConsumerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DisassociateDataShareConsumerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DisassociateDataShareConsumer(*pRequest), context);
    } );
}
EnableLoggingOutcome RedshiftClient::EnableLogging(const EnableLoggingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EnableLogging, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EnableLogging, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return EnableLoggingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

EnableLoggingOutcomeCallable RedshiftClient::EnableLoggingCallable(const EnableLoggingRequest& request) const
{
  std::shared_ptr<EnableLoggingRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< EnableLoggingOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->EnableLogging(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::EnableLoggingAsync(const EnableLoggingRequest& request, const EnableLoggingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<EnableLoggingRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, EnableLogging(*pRequest), context);
    } );
}
EnableSnapshotCopyOutcome RedshiftClient::EnableSnapshotCopy(const EnableSnapshotCopyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EnableSnapshotCopy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EnableSnapshotCopy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return EnableSnapshotCopyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

EnableSnapshotCopyOutcomeCallable RedshiftClient::EnableSnapshotCopyCallable(const EnableSnapshotCopyRequest& request) const
{
  std::shared_ptr<EnableSnapshotCopyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< EnableSnapshotCopyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->EnableSnapshotCopy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::EnableSnapshotCopyAsync(const EnableSnapshotCopyRequest& request, const EnableSnapshotCopyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<EnableSnapshotCopyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, EnableSnapshotCopy(*pRequest), context);
    } );
}
GetClusterCredentialsOutcome RedshiftClient::GetClusterCredentials(const GetClusterCredentialsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetClusterCredentials, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetClusterCredentials, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetClusterCredentialsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

GetClusterCredentialsOutcomeCallable RedshiftClient::GetClusterCredentialsCallable(const GetClusterCredentialsRequest& request) const
{
  std::shared_ptr<GetClusterCredentialsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetClusterCredentialsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetClusterCredentials(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::GetClusterCredentialsAsync(const GetClusterCredentialsRequest& request, const GetClusterCredentialsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetClusterCredentialsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetClusterCredentials(*pRequest), context);
    } );
}
GetClusterCredentialsWithIAMOutcome RedshiftClient::GetClusterCredentialsWithIAM(const GetClusterCredentialsWithIAMRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetClusterCredentialsWithIAM, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetClusterCredentialsWithIAM, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetClusterCredentialsWithIAMOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

GetClusterCredentialsWithIAMOutcomeCallable RedshiftClient::GetClusterCredentialsWithIAMCallable(const GetClusterCredentialsWithIAMRequest& request) const
{
  std::shared_ptr<GetClusterCredentialsWithIAMRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetClusterCredentialsWithIAMOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetClusterCredentialsWithIAM(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::GetClusterCredentialsWithIAMAsync(const GetClusterCredentialsWithIAMRequest& request, const GetClusterCredentialsWithIAMResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetClusterCredentialsWithIAMRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetClusterCredentialsWithIAM(*pRequest), context);
    } );
}
GetReservedNodeExchangeConfigurationOptionsOutcome RedshiftClient::GetReservedNodeExchangeConfigurationOptions(const GetReservedNodeExchangeConfigurationOptionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetReservedNodeExchangeConfigurationOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetReservedNodeExchangeConfigurationOptions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetReservedNodeExchangeConfigurationOptionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

GetReservedNodeExchangeConfigurationOptionsOutcomeCallable RedshiftClient::GetReservedNodeExchangeConfigurationOptionsCallable(const GetReservedNodeExchangeConfigurationOptionsRequest& request) const
{
  std::shared_ptr<GetReservedNodeExchangeConfigurationOptionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetReservedNodeExchangeConfigurationOptionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetReservedNodeExchangeConfigurationOptions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::GetReservedNodeExchangeConfigurationOptionsAsync(const GetReservedNodeExchangeConfigurationOptionsRequest& request, const GetReservedNodeExchangeConfigurationOptionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetReservedNodeExchangeConfigurationOptionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetReservedNodeExchangeConfigurationOptions(*pRequest), context);
    } );
}
GetReservedNodeExchangeOfferingsOutcome RedshiftClient::GetReservedNodeExchangeOfferings(const GetReservedNodeExchangeOfferingsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetReservedNodeExchangeOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetReservedNodeExchangeOfferings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetReservedNodeExchangeOfferingsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

GetReservedNodeExchangeOfferingsOutcomeCallable RedshiftClient::GetReservedNodeExchangeOfferingsCallable(const GetReservedNodeExchangeOfferingsRequest& request) const
{
  std::shared_ptr<GetReservedNodeExchangeOfferingsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetReservedNodeExchangeOfferingsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetReservedNodeExchangeOfferings(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::GetReservedNodeExchangeOfferingsAsync(const GetReservedNodeExchangeOfferingsRequest& request, const GetReservedNodeExchangeOfferingsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetReservedNodeExchangeOfferingsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetReservedNodeExchangeOfferings(*pRequest), context);
    } );
}
ModifyAquaConfigurationOutcome RedshiftClient::ModifyAquaConfiguration(const ModifyAquaConfigurationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyAquaConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyAquaConfiguration, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyAquaConfigurationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyAquaConfigurationOutcomeCallable RedshiftClient::ModifyAquaConfigurationCallable(const ModifyAquaConfigurationRequest& request) const
{
  std::shared_ptr<ModifyAquaConfigurationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyAquaConfigurationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyAquaConfiguration(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyAquaConfigurationAsync(const ModifyAquaConfigurationRequest& request, const ModifyAquaConfigurationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyAquaConfigurationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyAquaConfiguration(*pRequest), context);
    } );
}
ModifyAuthenticationProfileOutcome RedshiftClient::ModifyAuthenticationProfile(const ModifyAuthenticationProfileRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyAuthenticationProfile, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyAuthenticationProfileOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyAuthenticationProfileOutcomeCallable RedshiftClient::ModifyAuthenticationProfileCallable(const ModifyAuthenticationProfileRequest& request) const
{
  std::shared_ptr<ModifyAuthenticationProfileRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyAuthenticationProfileOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyAuthenticationProfile(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyAuthenticationProfileAsync(const ModifyAuthenticationProfileRequest& request, const ModifyAuthenticationProfileResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyAuthenticationProfileRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyAuthenticationProfile(*pRequest), context);
    } );
}
ModifyClusterOutcome RedshiftClient::ModifyCluster(const ModifyClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterOutcomeCallable RedshiftClient::ModifyClusterCallable(const ModifyClusterRequest& request) const
{
  std::shared_ptr<ModifyClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterAsync(const ModifyClusterRequest& request, const ModifyClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyCluster(*pRequest), context);
    } );
}
ModifyClusterDbRevisionOutcome RedshiftClient::ModifyClusterDbRevision(const ModifyClusterDbRevisionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterDbRevision, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterDbRevision, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterDbRevisionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterDbRevisionOutcomeCallable RedshiftClient::ModifyClusterDbRevisionCallable(const ModifyClusterDbRevisionRequest& request) const
{
  std::shared_ptr<ModifyClusterDbRevisionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterDbRevisionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterDbRevision(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterDbRevisionAsync(const ModifyClusterDbRevisionRequest& request, const ModifyClusterDbRevisionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterDbRevisionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterDbRevision(*pRequest), context);
    } );
}
ModifyClusterIamRolesOutcome RedshiftClient::ModifyClusterIamRoles(const ModifyClusterIamRolesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterIamRoles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterIamRoles, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterIamRolesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterIamRolesOutcomeCallable RedshiftClient::ModifyClusterIamRolesCallable(const ModifyClusterIamRolesRequest& request) const
{
  std::shared_ptr<ModifyClusterIamRolesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterIamRolesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterIamRoles(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterIamRolesAsync(const ModifyClusterIamRolesRequest& request, const ModifyClusterIamRolesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterIamRolesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterIamRoles(*pRequest), context);
    } );
}
ModifyClusterMaintenanceOutcome RedshiftClient::ModifyClusterMaintenance(const ModifyClusterMaintenanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterMaintenance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterMaintenance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterMaintenanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterMaintenanceOutcomeCallable RedshiftClient::ModifyClusterMaintenanceCallable(const ModifyClusterMaintenanceRequest& request) const
{
  std::shared_ptr<ModifyClusterMaintenanceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterMaintenanceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterMaintenance(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterMaintenanceAsync(const ModifyClusterMaintenanceRequest& request, const ModifyClusterMaintenanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterMaintenanceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterMaintenance(*pRequest), context);
    } );
}
ModifyClusterParameterGroupOutcome RedshiftClient::ModifyClusterParameterGroup(const ModifyClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterParameterGroupOutcomeCallable RedshiftClient::ModifyClusterParameterGroupCallable(const ModifyClusterParameterGroupRequest& request) const
{
  std::shared_ptr<ModifyClusterParameterGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterParameterGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterParameterGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterParameterGroupAsync(const ModifyClusterParameterGroupRequest& request, const ModifyClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterParameterGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterParameterGroup(*pRequest), context);
    } );
}
ModifyClusterSnapshotOutcome RedshiftClient::ModifyClusterSnapshot(const ModifyClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterSnapshotOutcomeCallable RedshiftClient::ModifyClusterSnapshotCallable(const ModifyClusterSnapshotRequest& request) const
{
  std::shared_ptr<ModifyClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterSnapshotAsync(const ModifyClusterSnapshotRequest& request, const ModifyClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterSnapshot(*pRequest), context);
    } );
}
ModifyClusterSnapshotScheduleOutcome RedshiftClient::ModifyClusterSnapshotSchedule(const ModifyClusterSnapshotScheduleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterSnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterSnapshotScheduleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterSnapshotScheduleOutcomeCallable RedshiftClient::ModifyClusterSnapshotScheduleCallable(const ModifyClusterSnapshotScheduleRequest& request) const
{
  std::shared_ptr<ModifyClusterSnapshotScheduleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterSnapshotScheduleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterSnapshotSchedule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterSnapshotScheduleAsync(const ModifyClusterSnapshotScheduleRequest& request, const ModifyClusterSnapshotScheduleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterSnapshotScheduleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterSnapshotSchedule(*pRequest), context);
    } );
}
ModifyClusterSubnetGroupOutcome RedshiftClient::ModifyClusterSubnetGroup(const ModifyClusterSubnetGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyClusterSubnetGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyClusterSubnetGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyClusterSubnetGroupOutcomeCallable RedshiftClient::ModifyClusterSubnetGroupCallable(const ModifyClusterSubnetGroupRequest& request) const
{
  std::shared_ptr<ModifyClusterSubnetGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyClusterSubnetGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyClusterSubnetGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyClusterSubnetGroupAsync(const ModifyClusterSubnetGroupRequest& request, const ModifyClusterSubnetGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyClusterSubnetGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyClusterSubnetGroup(*pRequest), context);
    } );
}
ModifyEndpointAccessOutcome RedshiftClient::ModifyEndpointAccess(const ModifyEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyEndpointAccessOutcomeCallable RedshiftClient::ModifyEndpointAccessCallable(const ModifyEndpointAccessRequest& request) const
{
  std::shared_ptr<ModifyEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyEndpointAccessAsync(const ModifyEndpointAccessRequest& request, const ModifyEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyEndpointAccess(*pRequest), context);
    } );
}
ModifyEventSubscriptionOutcome RedshiftClient::ModifyEventSubscription(const ModifyEventSubscriptionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyEventSubscription, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyEventSubscriptionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyEventSubscriptionOutcomeCallable RedshiftClient::ModifyEventSubscriptionCallable(const ModifyEventSubscriptionRequest& request) const
{
  std::shared_ptr<ModifyEventSubscriptionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyEventSubscriptionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyEventSubscription(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyEventSubscriptionAsync(const ModifyEventSubscriptionRequest& request, const ModifyEventSubscriptionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyEventSubscriptionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyEventSubscription(*pRequest), context);
    } );
}
ModifyScheduledActionOutcome RedshiftClient::ModifyScheduledAction(const ModifyScheduledActionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyScheduledAction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyScheduledActionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyScheduledActionOutcomeCallable RedshiftClient::ModifyScheduledActionCallable(const ModifyScheduledActionRequest& request) const
{
  std::shared_ptr<ModifyScheduledActionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyScheduledActionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyScheduledAction(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyScheduledActionAsync(const ModifyScheduledActionRequest& request, const ModifyScheduledActionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyScheduledActionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyScheduledAction(*pRequest), context);
    } );
}
ModifySnapshotCopyRetentionPeriodOutcome RedshiftClient::ModifySnapshotCopyRetentionPeriod(const ModifySnapshotCopyRetentionPeriodRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifySnapshotCopyRetentionPeriod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifySnapshotCopyRetentionPeriod, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifySnapshotCopyRetentionPeriodOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifySnapshotCopyRetentionPeriodOutcomeCallable RedshiftClient::ModifySnapshotCopyRetentionPeriodCallable(const ModifySnapshotCopyRetentionPeriodRequest& request) const
{
  std::shared_ptr<ModifySnapshotCopyRetentionPeriodRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifySnapshotCopyRetentionPeriodOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifySnapshotCopyRetentionPeriod(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifySnapshotCopyRetentionPeriodAsync(const ModifySnapshotCopyRetentionPeriodRequest& request, const ModifySnapshotCopyRetentionPeriodResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifySnapshotCopyRetentionPeriodRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifySnapshotCopyRetentionPeriod(*pRequest), context);
    } );
}
ModifySnapshotScheduleOutcome RedshiftClient::ModifySnapshotSchedule(const ModifySnapshotScheduleRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifySnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifySnapshotSchedule, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifySnapshotScheduleOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifySnapshotScheduleOutcomeCallable RedshiftClient::ModifySnapshotScheduleCallable(const ModifySnapshotScheduleRequest& request) const
{
  std::shared_ptr<ModifySnapshotScheduleRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifySnapshotScheduleOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifySnapshotSchedule(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifySnapshotScheduleAsync(const ModifySnapshotScheduleRequest& request, const ModifySnapshotScheduleResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifySnapshotScheduleRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifySnapshotSchedule(*pRequest), context);
    } );
}
ModifyUsageLimitOutcome RedshiftClient::ModifyUsageLimit(const ModifyUsageLimitRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ModifyUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ModifyUsageLimit, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ModifyUsageLimitOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ModifyUsageLimitOutcomeCallable RedshiftClient::ModifyUsageLimitCallable(const ModifyUsageLimitRequest& request) const
{
  std::shared_ptr<ModifyUsageLimitRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ModifyUsageLimitOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ModifyUsageLimit(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ModifyUsageLimitAsync(const ModifyUsageLimitRequest& request, const ModifyUsageLimitResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ModifyUsageLimitRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ModifyUsageLimit(*pRequest), context);
    } );
}
PauseClusterOutcome RedshiftClient::PauseCluster(const PauseClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PauseCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PauseCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PauseClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

PauseClusterOutcomeCallable RedshiftClient::PauseClusterCallable(const PauseClusterRequest& request) const
{
  std::shared_ptr<PauseClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< PauseClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->PauseCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::PauseClusterAsync(const PauseClusterRequest& request, const PauseClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<PauseClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, PauseCluster(*pRequest), context);
    } );
}
PurchaseReservedNodeOfferingOutcome RedshiftClient::PurchaseReservedNodeOffering(const PurchaseReservedNodeOfferingRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PurchaseReservedNodeOffering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PurchaseReservedNodeOffering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return PurchaseReservedNodeOfferingOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

PurchaseReservedNodeOfferingOutcomeCallable RedshiftClient::PurchaseReservedNodeOfferingCallable(const PurchaseReservedNodeOfferingRequest& request) const
{
  std::shared_ptr<PurchaseReservedNodeOfferingRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< PurchaseReservedNodeOfferingOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->PurchaseReservedNodeOffering(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::PurchaseReservedNodeOfferingAsync(const PurchaseReservedNodeOfferingRequest& request, const PurchaseReservedNodeOfferingResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<PurchaseReservedNodeOfferingRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, PurchaseReservedNodeOffering(*pRequest), context);
    } );
}
RebootClusterOutcome RedshiftClient::RebootCluster(const RebootClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RebootCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RebootCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RebootClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RebootClusterOutcomeCallable RedshiftClient::RebootClusterCallable(const RebootClusterRequest& request) const
{
  std::shared_ptr<RebootClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RebootClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RebootCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RebootClusterAsync(const RebootClusterRequest& request, const RebootClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RebootClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RebootCluster(*pRequest), context);
    } );
}
RejectDataShareOutcome RedshiftClient::RejectDataShare(const RejectDataShareRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RejectDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RejectDataShare, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RejectDataShareOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RejectDataShareOutcomeCallable RedshiftClient::RejectDataShareCallable(const RejectDataShareRequest& request) const
{
  std::shared_ptr<RejectDataShareRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RejectDataShareOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RejectDataShare(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RejectDataShareAsync(const RejectDataShareRequest& request, const RejectDataShareResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RejectDataShareRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RejectDataShare(*pRequest), context);
    } );
}
ResetClusterParameterGroupOutcome RedshiftClient::ResetClusterParameterGroup(const ResetClusterParameterGroupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResetClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResetClusterParameterGroup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResetClusterParameterGroupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ResetClusterParameterGroupOutcomeCallable RedshiftClient::ResetClusterParameterGroupCallable(const ResetClusterParameterGroupRequest& request) const
{
  std::shared_ptr<ResetClusterParameterGroupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ResetClusterParameterGroupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ResetClusterParameterGroup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ResetClusterParameterGroupAsync(const ResetClusterParameterGroupRequest& request, const ResetClusterParameterGroupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ResetClusterParameterGroupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ResetClusterParameterGroup(*pRequest), context);
    } );
}
ResizeClusterOutcome RedshiftClient::ResizeCluster(const ResizeClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResizeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResizeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResizeClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ResizeClusterOutcomeCallable RedshiftClient::ResizeClusterCallable(const ResizeClusterRequest& request) const
{
  std::shared_ptr<ResizeClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ResizeClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ResizeCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ResizeClusterAsync(const ResizeClusterRequest& request, const ResizeClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ResizeClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ResizeCluster(*pRequest), context);
    } );
}
RestoreFromClusterSnapshotOutcome RedshiftClient::RestoreFromClusterSnapshot(const RestoreFromClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreFromClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreFromClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreFromClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreFromClusterSnapshotOutcomeCallable RedshiftClient::RestoreFromClusterSnapshotCallable(const RestoreFromClusterSnapshotRequest& request) const
{
  std::shared_ptr<RestoreFromClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RestoreFromClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RestoreFromClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RestoreFromClusterSnapshotAsync(const RestoreFromClusterSnapshotRequest& request, const RestoreFromClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RestoreFromClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RestoreFromClusterSnapshot(*pRequest), context);
    } );
}
RestoreTableFromClusterSnapshotOutcome RedshiftClient::RestoreTableFromClusterSnapshot(const RestoreTableFromClusterSnapshotRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreTableFromClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreTableFromClusterSnapshot, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreTableFromClusterSnapshotOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RestoreTableFromClusterSnapshotOutcomeCallable RedshiftClient::RestoreTableFromClusterSnapshotCallable(const RestoreTableFromClusterSnapshotRequest& request) const
{
  std::shared_ptr<RestoreTableFromClusterSnapshotRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RestoreTableFromClusterSnapshotOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RestoreTableFromClusterSnapshot(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RestoreTableFromClusterSnapshotAsync(const RestoreTableFromClusterSnapshotRequest& request, const RestoreTableFromClusterSnapshotResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RestoreTableFromClusterSnapshotRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RestoreTableFromClusterSnapshot(*pRequest), context);
    } );
}
ResumeClusterOutcome RedshiftClient::ResumeCluster(const ResumeClusterRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResumeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResumeCluster, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResumeClusterOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

ResumeClusterOutcomeCallable RedshiftClient::ResumeClusterCallable(const ResumeClusterRequest& request) const
{
  std::shared_ptr<ResumeClusterRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ResumeClusterOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ResumeCluster(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::ResumeClusterAsync(const ResumeClusterRequest& request, const ResumeClusterResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ResumeClusterRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ResumeCluster(*pRequest), context);
    } );
}
RevokeClusterSecurityGroupIngressOutcome RedshiftClient::RevokeClusterSecurityGroupIngress(const RevokeClusterSecurityGroupIngressRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeClusterSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeClusterSecurityGroupIngress, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeClusterSecurityGroupIngressOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RevokeClusterSecurityGroupIngressOutcomeCallable RedshiftClient::RevokeClusterSecurityGroupIngressCallable(const RevokeClusterSecurityGroupIngressRequest& request) const
{
  std::shared_ptr<RevokeClusterSecurityGroupIngressRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RevokeClusterSecurityGroupIngressOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RevokeClusterSecurityGroupIngress(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RevokeClusterSecurityGroupIngressAsync(const RevokeClusterSecurityGroupIngressRequest& request, const RevokeClusterSecurityGroupIngressResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RevokeClusterSecurityGroupIngressRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RevokeClusterSecurityGroupIngress(*pRequest), context);
    } );
}
RevokeEndpointAccessOutcome RedshiftClient::RevokeEndpointAccess(const RevokeEndpointAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeEndpointAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeEndpointAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RevokeEndpointAccessOutcomeCallable RedshiftClient::RevokeEndpointAccessCallable(const RevokeEndpointAccessRequest& request) const
{
  std::shared_ptr<RevokeEndpointAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RevokeEndpointAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RevokeEndpointAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RevokeEndpointAccessAsync(const RevokeEndpointAccessRequest& request, const RevokeEndpointAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RevokeEndpointAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RevokeEndpointAccess(*pRequest), context);
    } );
}
RevokeSnapshotAccessOutcome RedshiftClient::RevokeSnapshotAccess(const RevokeSnapshotAccessRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RevokeSnapshotAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RevokeSnapshotAccess, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RevokeSnapshotAccessOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RevokeSnapshotAccessOutcomeCallable RedshiftClient::RevokeSnapshotAccessCallable(const RevokeSnapshotAccessRequest& request) const
{
  std::shared_ptr<RevokeSnapshotAccessRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RevokeSnapshotAccessOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RevokeSnapshotAccess(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RevokeSnapshotAccessAsync(const RevokeSnapshotAccessRequest& request, const RevokeSnapshotAccessResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RevokeSnapshotAccessRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RevokeSnapshotAccess(*pRequest), context);
    } );
}
RotateEncryptionKeyOutcome RedshiftClient::RotateEncryptionKey(const RotateEncryptionKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RotateEncryptionKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RotateEncryptionKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RotateEncryptionKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

RotateEncryptionKeyOutcomeCallable RedshiftClient::RotateEncryptionKeyCallable(const RotateEncryptionKeyRequest& request) const
{
  std::shared_ptr<RotateEncryptionKeyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RotateEncryptionKeyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RotateEncryptionKey(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::RotateEncryptionKeyAsync(const RotateEncryptionKeyRequest& request, const RotateEncryptionKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RotateEncryptionKeyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RotateEncryptionKey(*pRequest), context);
    } );
}
UpdatePartnerStatusOutcome RedshiftClient::UpdatePartnerStatus(const UpdatePartnerStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdatePartnerStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdatePartnerStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdatePartnerStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST));
}

UpdatePartnerStatusOutcomeCallable RedshiftClient::UpdatePartnerStatusCallable(const UpdatePartnerStatusRequest& request) const
{
  std::shared_ptr<UpdatePartnerStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdatePartnerStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdatePartnerStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RedshiftClient::UpdatePartnerStatusAsync(const UpdatePartnerStatusRequest& request, const UpdatePartnerStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdatePartnerStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdatePartnerStatus(*pRequest), context);
    } );
}
