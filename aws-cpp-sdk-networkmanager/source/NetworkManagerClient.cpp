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

#include <aws/networkmanager/NetworkManagerClient.h>
#include <aws/networkmanager/NetworkManagerErrorMarshaller.h>
#include <aws/networkmanager/NetworkManagerEndpointProvider.h>
#include <aws/networkmanager/model/AcceptAttachmentRequest.h>
#include <aws/networkmanager/model/AssociateConnectPeerRequest.h>
#include <aws/networkmanager/model/AssociateCustomerGatewayRequest.h>
#include <aws/networkmanager/model/AssociateLinkRequest.h>
#include <aws/networkmanager/model/AssociateTransitGatewayConnectPeerRequest.h>
#include <aws/networkmanager/model/CreateConnectAttachmentRequest.h>
#include <aws/networkmanager/model/CreateConnectPeerRequest.h>
#include <aws/networkmanager/model/CreateConnectionRequest.h>
#include <aws/networkmanager/model/CreateCoreNetworkRequest.h>
#include <aws/networkmanager/model/CreateDeviceRequest.h>
#include <aws/networkmanager/model/CreateGlobalNetworkRequest.h>
#include <aws/networkmanager/model/CreateLinkRequest.h>
#include <aws/networkmanager/model/CreateSiteRequest.h>
#include <aws/networkmanager/model/CreateSiteToSiteVpnAttachmentRequest.h>
#include <aws/networkmanager/model/CreateTransitGatewayPeeringRequest.h>
#include <aws/networkmanager/model/CreateTransitGatewayRouteTableAttachmentRequest.h>
#include <aws/networkmanager/model/CreateVpcAttachmentRequest.h>
#include <aws/networkmanager/model/DeleteAttachmentRequest.h>
#include <aws/networkmanager/model/DeleteConnectPeerRequest.h>
#include <aws/networkmanager/model/DeleteConnectionRequest.h>
#include <aws/networkmanager/model/DeleteCoreNetworkRequest.h>
#include <aws/networkmanager/model/DeleteCoreNetworkPolicyVersionRequest.h>
#include <aws/networkmanager/model/DeleteDeviceRequest.h>
#include <aws/networkmanager/model/DeleteGlobalNetworkRequest.h>
#include <aws/networkmanager/model/DeleteLinkRequest.h>
#include <aws/networkmanager/model/DeletePeeringRequest.h>
#include <aws/networkmanager/model/DeleteResourcePolicyRequest.h>
#include <aws/networkmanager/model/DeleteSiteRequest.h>
#include <aws/networkmanager/model/DeregisterTransitGatewayRequest.h>
#include <aws/networkmanager/model/DescribeGlobalNetworksRequest.h>
#include <aws/networkmanager/model/DisassociateConnectPeerRequest.h>
#include <aws/networkmanager/model/DisassociateCustomerGatewayRequest.h>
#include <aws/networkmanager/model/DisassociateLinkRequest.h>
#include <aws/networkmanager/model/DisassociateTransitGatewayConnectPeerRequest.h>
#include <aws/networkmanager/model/ExecuteCoreNetworkChangeSetRequest.h>
#include <aws/networkmanager/model/GetConnectAttachmentRequest.h>
#include <aws/networkmanager/model/GetConnectPeerRequest.h>
#include <aws/networkmanager/model/GetConnectPeerAssociationsRequest.h>
#include <aws/networkmanager/model/GetConnectionsRequest.h>
#include <aws/networkmanager/model/GetCoreNetworkRequest.h>
#include <aws/networkmanager/model/GetCoreNetworkChangeEventsRequest.h>
#include <aws/networkmanager/model/GetCoreNetworkChangeSetRequest.h>
#include <aws/networkmanager/model/GetCoreNetworkPolicyRequest.h>
#include <aws/networkmanager/model/GetCustomerGatewayAssociationsRequest.h>
#include <aws/networkmanager/model/GetDevicesRequest.h>
#include <aws/networkmanager/model/GetLinkAssociationsRequest.h>
#include <aws/networkmanager/model/GetLinksRequest.h>
#include <aws/networkmanager/model/GetNetworkResourceCountsRequest.h>
#include <aws/networkmanager/model/GetNetworkResourceRelationshipsRequest.h>
#include <aws/networkmanager/model/GetNetworkResourcesRequest.h>
#include <aws/networkmanager/model/GetNetworkRoutesRequest.h>
#include <aws/networkmanager/model/GetNetworkTelemetryRequest.h>
#include <aws/networkmanager/model/GetResourcePolicyRequest.h>
#include <aws/networkmanager/model/GetRouteAnalysisRequest.h>
#include <aws/networkmanager/model/GetSiteToSiteVpnAttachmentRequest.h>
#include <aws/networkmanager/model/GetSitesRequest.h>
#include <aws/networkmanager/model/GetTransitGatewayConnectPeerAssociationsRequest.h>
#include <aws/networkmanager/model/GetTransitGatewayPeeringRequest.h>
#include <aws/networkmanager/model/GetTransitGatewayRegistrationsRequest.h>
#include <aws/networkmanager/model/GetTransitGatewayRouteTableAttachmentRequest.h>
#include <aws/networkmanager/model/GetVpcAttachmentRequest.h>
#include <aws/networkmanager/model/ListAttachmentsRequest.h>
#include <aws/networkmanager/model/ListConnectPeersRequest.h>
#include <aws/networkmanager/model/ListCoreNetworkPolicyVersionsRequest.h>
#include <aws/networkmanager/model/ListCoreNetworksRequest.h>
#include <aws/networkmanager/model/ListOrganizationServiceAccessStatusRequest.h>
#include <aws/networkmanager/model/ListPeeringsRequest.h>
#include <aws/networkmanager/model/ListTagsForResourceRequest.h>
#include <aws/networkmanager/model/PutCoreNetworkPolicyRequest.h>
#include <aws/networkmanager/model/PutResourcePolicyRequest.h>
#include <aws/networkmanager/model/RegisterTransitGatewayRequest.h>
#include <aws/networkmanager/model/RejectAttachmentRequest.h>
#include <aws/networkmanager/model/RestoreCoreNetworkPolicyVersionRequest.h>
#include <aws/networkmanager/model/StartOrganizationServiceAccessUpdateRequest.h>
#include <aws/networkmanager/model/StartRouteAnalysisRequest.h>
#include <aws/networkmanager/model/TagResourceRequest.h>
#include <aws/networkmanager/model/UntagResourceRequest.h>
#include <aws/networkmanager/model/UpdateConnectionRequest.h>
#include <aws/networkmanager/model/UpdateCoreNetworkRequest.h>
#include <aws/networkmanager/model/UpdateDeviceRequest.h>
#include <aws/networkmanager/model/UpdateGlobalNetworkRequest.h>
#include <aws/networkmanager/model/UpdateLinkRequest.h>
#include <aws/networkmanager/model/UpdateNetworkResourceMetadataRequest.h>
#include <aws/networkmanager/model/UpdateSiteRequest.h>
#include <aws/networkmanager/model/UpdateVpcAttachmentRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::NetworkManager;
using namespace Aws::NetworkManager::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* NetworkManagerClient::SERVICE_NAME = "networkmanager";
const char* NetworkManagerClient::ALLOCATION_TAG = "NetworkManagerClient";

NetworkManagerClient::NetworkManagerClient(const NetworkManager::NetworkManagerClientConfiguration& clientConfiguration,
                                           std::shared_ptr<NetworkManagerEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

NetworkManagerClient::NetworkManagerClient(const AWSCredentials& credentials,
                                           std::shared_ptr<NetworkManagerEndpointProviderBase> endpointProvider,
                                           const NetworkManager::NetworkManagerClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

NetworkManagerClient::NetworkManagerClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           std::shared_ptr<NetworkManagerEndpointProviderBase> endpointProvider,
                                           const NetworkManager::NetworkManagerClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  NetworkManagerClient::NetworkManagerClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<NetworkManagerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

NetworkManagerClient::NetworkManagerClient(const AWSCredentials& credentials,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<NetworkManagerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

NetworkManagerClient::NetworkManagerClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                           const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<NetworkManagerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<NetworkManagerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
NetworkManagerClient::~NetworkManagerClient()
{
}

std::shared_ptr<NetworkManagerEndpointProviderBase>& NetworkManagerClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void NetworkManagerClient::init(const NetworkManager::NetworkManagerClientConfiguration& config)
{
  AWSClient::SetServiceClientName("NetworkManager");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void NetworkManagerClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AcceptAttachmentOutcome NetworkManagerClient::AcceptAttachment(const AcceptAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AcceptAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AcceptAttachment", "Required field: AttachmentId, is not set");
    return AcceptAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AcceptAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/accept");
  return AcceptAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AcceptAttachmentOutcomeCallable NetworkManagerClient::AcceptAttachmentCallable(const AcceptAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AcceptAttachment, request, m_executor.get());
}

void NetworkManagerClient::AcceptAttachmentAsync(const AcceptAttachmentRequest& request, const AcceptAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AcceptAttachment, request, handler, context, m_executor.get());
}

AssociateConnectPeerOutcome NetworkManagerClient::AssociateConnectPeer(const AssociateConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateConnectPeer", "Required field: GlobalNetworkId, is not set");
    return AssociateConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peer-associations");
  return AssociateConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateConnectPeerOutcomeCallable NetworkManagerClient::AssociateConnectPeerCallable(const AssociateConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::AssociateConnectPeerAsync(const AssociateConnectPeerRequest& request, const AssociateConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateConnectPeer, request, handler, context, m_executor.get());
}

AssociateCustomerGatewayOutcome NetworkManagerClient::AssociateCustomerGateway(const AssociateCustomerGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateCustomerGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateCustomerGateway", "Required field: GlobalNetworkId, is not set");
    return AssociateCustomerGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateCustomerGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/customer-gateway-associations");
  return AssociateCustomerGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateCustomerGatewayOutcomeCallable NetworkManagerClient::AssociateCustomerGatewayCallable(const AssociateCustomerGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateCustomerGateway, request, m_executor.get());
}

void NetworkManagerClient::AssociateCustomerGatewayAsync(const AssociateCustomerGatewayRequest& request, const AssociateCustomerGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateCustomerGateway, request, handler, context, m_executor.get());
}

AssociateLinkOutcome NetworkManagerClient::AssociateLink(const AssociateLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateLink", "Required field: GlobalNetworkId, is not set");
    return AssociateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/link-associations");
  return AssociateLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateLinkOutcomeCallable NetworkManagerClient::AssociateLinkCallable(const AssociateLinkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateLink, request, m_executor.get());
}

void NetworkManagerClient::AssociateLinkAsync(const AssociateLinkRequest& request, const AssociateLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateLink, request, handler, context, m_executor.get());
}

AssociateTransitGatewayConnectPeerOutcome NetworkManagerClient::AssociateTransitGatewayConnectPeer(const AssociateTransitGatewayConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateTransitGatewayConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateTransitGatewayConnectPeer", "Required field: GlobalNetworkId, is not set");
    return AssociateTransitGatewayConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateTransitGatewayConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-connect-peer-associations");
  return AssociateTransitGatewayConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateTransitGatewayConnectPeerOutcomeCallable NetworkManagerClient::AssociateTransitGatewayConnectPeerCallable(const AssociateTransitGatewayConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateTransitGatewayConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::AssociateTransitGatewayConnectPeerAsync(const AssociateTransitGatewayConnectPeerRequest& request, const AssociateTransitGatewayConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateTransitGatewayConnectPeer, request, handler, context, m_executor.get());
}

CreateConnectAttachmentOutcome NetworkManagerClient::CreateConnectAttachment(const CreateConnectAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConnectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConnectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-attachments");
  return CreateConnectAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConnectAttachmentOutcomeCallable NetworkManagerClient::CreateConnectAttachmentCallable(const CreateConnectAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateConnectAttachment, request, m_executor.get());
}

void NetworkManagerClient::CreateConnectAttachmentAsync(const CreateConnectAttachmentRequest& request, const CreateConnectAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateConnectAttachment, request, handler, context, m_executor.get());
}

CreateConnectPeerOutcome NetworkManagerClient::CreateConnectPeer(const CreateConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peers");
  return CreateConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConnectPeerOutcomeCallable NetworkManagerClient::CreateConnectPeerCallable(const CreateConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::CreateConnectPeerAsync(const CreateConnectPeerRequest& request, const CreateConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateConnectPeer, request, handler, context, m_executor.get());
}

CreateConnectionOutcome NetworkManagerClient::CreateConnection(const CreateConnectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateConnection", "Required field: GlobalNetworkId, is not set");
    return CreateConnectionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connections");
  return CreateConnectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateConnectionOutcomeCallable NetworkManagerClient::CreateConnectionCallable(const CreateConnectionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateConnection, request, m_executor.get());
}

void NetworkManagerClient::CreateConnectionAsync(const CreateConnectionRequest& request, const CreateConnectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateConnection, request, handler, context, m_executor.get());
}

CreateCoreNetworkOutcome NetworkManagerClient::CreateCoreNetwork(const CreateCoreNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks");
  return CreateCoreNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateCoreNetworkOutcomeCallable NetworkManagerClient::CreateCoreNetworkCallable(const CreateCoreNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateCoreNetwork, request, m_executor.get());
}

void NetworkManagerClient::CreateCoreNetworkAsync(const CreateCoreNetworkRequest& request, const CreateCoreNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateCoreNetwork, request, handler, context, m_executor.get());
}

CreateDeviceOutcome NetworkManagerClient::CreateDevice(const CreateDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDevice", "Required field: GlobalNetworkId, is not set");
    return CreateDeviceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/devices");
  return CreateDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDeviceOutcomeCallable NetworkManagerClient::CreateDeviceCallable(const CreateDeviceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateDevice, request, m_executor.get());
}

void NetworkManagerClient::CreateDeviceAsync(const CreateDeviceRequest& request, const CreateDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateDevice, request, handler, context, m_executor.get());
}

CreateGlobalNetworkOutcome NetworkManagerClient::CreateGlobalNetwork(const CreateGlobalNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks");
  return CreateGlobalNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGlobalNetworkOutcomeCallable NetworkManagerClient::CreateGlobalNetworkCallable(const CreateGlobalNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateGlobalNetwork, request, m_executor.get());
}

void NetworkManagerClient::CreateGlobalNetworkAsync(const CreateGlobalNetworkRequest& request, const CreateGlobalNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateGlobalNetwork, request, handler, context, m_executor.get());
}

CreateLinkOutcome NetworkManagerClient::CreateLink(const CreateLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateLink", "Required field: GlobalNetworkId, is not set");
    return CreateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/links");
  return CreateLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateLinkOutcomeCallable NetworkManagerClient::CreateLinkCallable(const CreateLinkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateLink, request, m_executor.get());
}

void NetworkManagerClient::CreateLinkAsync(const CreateLinkRequest& request, const CreateLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateLink, request, handler, context, m_executor.get());
}

CreateSiteOutcome NetworkManagerClient::CreateSite(const CreateSiteRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateSite", "Required field: GlobalNetworkId, is not set");
    return CreateSiteOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sites");
  return CreateSiteOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSiteOutcomeCallable NetworkManagerClient::CreateSiteCallable(const CreateSiteRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateSite, request, m_executor.get());
}

void NetworkManagerClient::CreateSiteAsync(const CreateSiteRequest& request, const CreateSiteResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateSite, request, handler, context, m_executor.get());
}

CreateSiteToSiteVpnAttachmentOutcome NetworkManagerClient::CreateSiteToSiteVpnAttachment(const CreateSiteToSiteVpnAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSiteToSiteVpnAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSiteToSiteVpnAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/site-to-site-vpn-attachments");
  return CreateSiteToSiteVpnAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSiteToSiteVpnAttachmentOutcomeCallable NetworkManagerClient::CreateSiteToSiteVpnAttachmentCallable(const CreateSiteToSiteVpnAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateSiteToSiteVpnAttachment, request, m_executor.get());
}

void NetworkManagerClient::CreateSiteToSiteVpnAttachmentAsync(const CreateSiteToSiteVpnAttachmentRequest& request, const CreateSiteToSiteVpnAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateSiteToSiteVpnAttachment, request, handler, context, m_executor.get());
}

CreateTransitGatewayPeeringOutcome NetworkManagerClient::CreateTransitGatewayPeering(const CreateTransitGatewayPeeringRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTransitGatewayPeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTransitGatewayPeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-peerings");
  return CreateTransitGatewayPeeringOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateTransitGatewayPeeringOutcomeCallable NetworkManagerClient::CreateTransitGatewayPeeringCallable(const CreateTransitGatewayPeeringRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateTransitGatewayPeering, request, m_executor.get());
}

void NetworkManagerClient::CreateTransitGatewayPeeringAsync(const CreateTransitGatewayPeeringRequest& request, const CreateTransitGatewayPeeringResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateTransitGatewayPeering, request, handler, context, m_executor.get());
}

CreateTransitGatewayRouteTableAttachmentOutcome NetworkManagerClient::CreateTransitGatewayRouteTableAttachment(const CreateTransitGatewayRouteTableAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTransitGatewayRouteTableAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTransitGatewayRouteTableAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-route-table-attachments");
  return CreateTransitGatewayRouteTableAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateTransitGatewayRouteTableAttachmentOutcomeCallable NetworkManagerClient::CreateTransitGatewayRouteTableAttachmentCallable(const CreateTransitGatewayRouteTableAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateTransitGatewayRouteTableAttachment, request, m_executor.get());
}

void NetworkManagerClient::CreateTransitGatewayRouteTableAttachmentAsync(const CreateTransitGatewayRouteTableAttachmentRequest& request, const CreateTransitGatewayRouteTableAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateTransitGatewayRouteTableAttachment, request, handler, context, m_executor.get());
}

CreateVpcAttachmentOutcome NetworkManagerClient::CreateVpcAttachment(const CreateVpcAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpc-attachments");
  return CreateVpcAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateVpcAttachmentOutcomeCallable NetworkManagerClient::CreateVpcAttachmentCallable(const CreateVpcAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateVpcAttachment, request, m_executor.get());
}

void NetworkManagerClient::CreateVpcAttachmentAsync(const CreateVpcAttachmentRequest& request, const CreateVpcAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateVpcAttachment, request, handler, context, m_executor.get());
}

DeleteAttachmentOutcome NetworkManagerClient::DeleteAttachment(const DeleteAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteAttachment", "Required field: AttachmentId, is not set");
    return DeleteAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return DeleteAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteAttachmentOutcomeCallable NetworkManagerClient::DeleteAttachmentCallable(const DeleteAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteAttachment, request, m_executor.get());
}

void NetworkManagerClient::DeleteAttachmentAsync(const DeleteAttachmentRequest& request, const DeleteAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteAttachment, request, handler, context, m_executor.get());
}

DeleteConnectPeerOutcome NetworkManagerClient::DeleteConnectPeer(const DeleteConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectPeerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConnectPeer", "Required field: ConnectPeerId, is not set");
    return DeleteConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectPeerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectPeerId());
  return DeleteConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteConnectPeerOutcomeCallable NetworkManagerClient::DeleteConnectPeerCallable(const DeleteConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::DeleteConnectPeerAsync(const DeleteConnectPeerRequest& request, const DeleteConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteConnectPeer, request, handler, context, m_executor.get());
}

DeleteConnectionOutcome NetworkManagerClient::DeleteConnection(const DeleteConnectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConnection", "Required field: GlobalNetworkId, is not set");
    return DeleteConnectionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.ConnectionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteConnection", "Required field: ConnectionId, is not set");
    return DeleteConnectionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connections/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectionId());
  return DeleteConnectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteConnectionOutcomeCallable NetworkManagerClient::DeleteConnectionCallable(const DeleteConnectionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteConnection, request, m_executor.get());
}

void NetworkManagerClient::DeleteConnectionAsync(const DeleteConnectionRequest& request, const DeleteConnectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteConnection, request, handler, context, m_executor.get());
}

DeleteCoreNetworkOutcome NetworkManagerClient::DeleteCoreNetwork(const DeleteCoreNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCoreNetwork", "Required field: CoreNetworkId, is not set");
    return DeleteCoreNetworkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  return DeleteCoreNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteCoreNetworkOutcomeCallable NetworkManagerClient::DeleteCoreNetworkCallable(const DeleteCoreNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteCoreNetwork, request, m_executor.get());
}

void NetworkManagerClient::DeleteCoreNetworkAsync(const DeleteCoreNetworkRequest& request, const DeleteCoreNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteCoreNetwork, request, handler, context, m_executor.get());
}

DeleteCoreNetworkPolicyVersionOutcome NetworkManagerClient::DeleteCoreNetworkPolicyVersion(const DeleteCoreNetworkPolicyVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteCoreNetworkPolicyVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCoreNetworkPolicyVersion", "Required field: CoreNetworkId, is not set");
    return DeleteCoreNetworkPolicyVersionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  if (!request.PolicyVersionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteCoreNetworkPolicyVersion", "Required field: PolicyVersionId, is not set");
    return DeleteCoreNetworkPolicyVersionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PolicyVersionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteCoreNetworkPolicyVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-policy-versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPolicyVersionId());
  return DeleteCoreNetworkPolicyVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteCoreNetworkPolicyVersionOutcomeCallable NetworkManagerClient::DeleteCoreNetworkPolicyVersionCallable(const DeleteCoreNetworkPolicyVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteCoreNetworkPolicyVersion, request, m_executor.get());
}

void NetworkManagerClient::DeleteCoreNetworkPolicyVersionAsync(const DeleteCoreNetworkPolicyVersionRequest& request, const DeleteCoreNetworkPolicyVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteCoreNetworkPolicyVersion, request, handler, context, m_executor.get());
}

DeleteDeviceOutcome NetworkManagerClient::DeleteDevice(const DeleteDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDevice", "Required field: GlobalNetworkId, is not set");
    return DeleteDeviceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.DeviceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDevice", "Required field: DeviceId, is not set");
    return DeleteDeviceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeviceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/devices/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeviceId());
  return DeleteDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDeviceOutcomeCallable NetworkManagerClient::DeleteDeviceCallable(const DeleteDeviceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteDevice, request, m_executor.get());
}

void NetworkManagerClient::DeleteDeviceAsync(const DeleteDeviceRequest& request, const DeleteDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteDevice, request, handler, context, m_executor.get());
}

DeleteGlobalNetworkOutcome NetworkManagerClient::DeleteGlobalNetwork(const DeleteGlobalNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGlobalNetwork", "Required field: GlobalNetworkId, is not set");
    return DeleteGlobalNetworkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  return DeleteGlobalNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteGlobalNetworkOutcomeCallable NetworkManagerClient::DeleteGlobalNetworkCallable(const DeleteGlobalNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteGlobalNetwork, request, m_executor.get());
}

void NetworkManagerClient::DeleteGlobalNetworkAsync(const DeleteGlobalNetworkRequest& request, const DeleteGlobalNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteGlobalNetwork, request, handler, context, m_executor.get());
}

DeleteLinkOutcome NetworkManagerClient::DeleteLink(const DeleteLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteLink", "Required field: GlobalNetworkId, is not set");
    return DeleteLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.LinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteLink", "Required field: LinkId, is not set");
    return DeleteLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [LinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/links/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLinkId());
  return DeleteLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteLinkOutcomeCallable NetworkManagerClient::DeleteLinkCallable(const DeleteLinkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteLink, request, m_executor.get());
}

void NetworkManagerClient::DeleteLinkAsync(const DeleteLinkRequest& request, const DeleteLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteLink, request, handler, context, m_executor.get());
}

DeletePeeringOutcome NetworkManagerClient::DeletePeering(const DeletePeeringRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeletePeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PeeringIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeletePeering", "Required field: PeeringId, is not set");
    return DeletePeeringOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PeeringId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeletePeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/peerings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPeeringId());
  return DeletePeeringOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeletePeeringOutcomeCallable NetworkManagerClient::DeletePeeringCallable(const DeletePeeringRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeletePeering, request, m_executor.get());
}

void NetworkManagerClient::DeletePeeringAsync(const DeletePeeringRequest& request, const DeletePeeringResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeletePeering, request, handler, context, m_executor.get());
}

DeleteResourcePolicyOutcome NetworkManagerClient::DeleteResourcePolicy(const DeleteResourcePolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResourcePolicy", "Required field: ResourceArn, is not set");
    return DeleteResourcePolicyOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resource-policy/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return DeleteResourcePolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteResourcePolicyOutcomeCallable NetworkManagerClient::DeleteResourcePolicyCallable(const DeleteResourcePolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteResourcePolicy, request, m_executor.get());
}

void NetworkManagerClient::DeleteResourcePolicyAsync(const DeleteResourcePolicyRequest& request, const DeleteResourcePolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteResourcePolicy, request, handler, context, m_executor.get());
}

DeleteSiteOutcome NetworkManagerClient::DeleteSite(const DeleteSiteRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSite", "Required field: GlobalNetworkId, is not set");
    return DeleteSiteOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.SiteIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteSite", "Required field: SiteId, is not set");
    return DeleteSiteOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SiteId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sites/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSiteId());
  return DeleteSiteOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteSiteOutcomeCallable NetworkManagerClient::DeleteSiteCallable(const DeleteSiteRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteSite, request, m_executor.get());
}

void NetworkManagerClient::DeleteSiteAsync(const DeleteSiteRequest& request, const DeleteSiteResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteSite, request, handler, context, m_executor.get());
}

DeregisterTransitGatewayOutcome NetworkManagerClient::DeregisterTransitGateway(const DeregisterTransitGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeregisterTransitGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeregisterTransitGateway", "Required field: GlobalNetworkId, is not set");
    return DeregisterTransitGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.TransitGatewayArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeregisterTransitGateway", "Required field: TransitGatewayArn, is not set");
    return DeregisterTransitGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TransitGatewayArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeregisterTransitGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-registrations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTransitGatewayArn());
  return DeregisterTransitGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeregisterTransitGatewayOutcomeCallable NetworkManagerClient::DeregisterTransitGatewayCallable(const DeregisterTransitGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeregisterTransitGateway, request, m_executor.get());
}

void NetworkManagerClient::DeregisterTransitGatewayAsync(const DeregisterTransitGatewayRequest& request, const DeregisterTransitGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeregisterTransitGateway, request, handler, context, m_executor.get());
}

DescribeGlobalNetworksOutcome NetworkManagerClient::DescribeGlobalNetworks(const DescribeGlobalNetworksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeGlobalNetworks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeGlobalNetworks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks");
  return DescribeGlobalNetworksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeGlobalNetworksOutcomeCallable NetworkManagerClient::DescribeGlobalNetworksCallable(const DescribeGlobalNetworksRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DescribeGlobalNetworks, request, m_executor.get());
}

void NetworkManagerClient::DescribeGlobalNetworksAsync(const DescribeGlobalNetworksRequest& request, const DescribeGlobalNetworksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DescribeGlobalNetworks, request, handler, context, m_executor.get());
}

DisassociateConnectPeerOutcome NetworkManagerClient::DisassociateConnectPeer(const DisassociateConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateConnectPeer", "Required field: GlobalNetworkId, is not set");
    return DisassociateConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.ConnectPeerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateConnectPeer", "Required field: ConnectPeerId, is not set");
    return DisassociateConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectPeerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peer-associations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectPeerId());
  return DisassociateConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateConnectPeerOutcomeCallable NetworkManagerClient::DisassociateConnectPeerCallable(const DisassociateConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::DisassociateConnectPeerAsync(const DisassociateConnectPeerRequest& request, const DisassociateConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateConnectPeer, request, handler, context, m_executor.get());
}

DisassociateCustomerGatewayOutcome NetworkManagerClient::DisassociateCustomerGateway(const DisassociateCustomerGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateCustomerGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateCustomerGateway", "Required field: GlobalNetworkId, is not set");
    return DisassociateCustomerGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.CustomerGatewayArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateCustomerGateway", "Required field: CustomerGatewayArn, is not set");
    return DisassociateCustomerGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CustomerGatewayArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateCustomerGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/customer-gateway-associations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCustomerGatewayArn());
  return DisassociateCustomerGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateCustomerGatewayOutcomeCallable NetworkManagerClient::DisassociateCustomerGatewayCallable(const DisassociateCustomerGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateCustomerGateway, request, m_executor.get());
}

void NetworkManagerClient::DisassociateCustomerGatewayAsync(const DisassociateCustomerGatewayRequest& request, const DisassociateCustomerGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateCustomerGateway, request, handler, context, m_executor.get());
}

DisassociateLinkOutcome NetworkManagerClient::DisassociateLink(const DisassociateLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateLink", "Required field: GlobalNetworkId, is not set");
    return DisassociateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.DeviceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateLink", "Required field: DeviceId, is not set");
    return DisassociateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeviceId]", false));
  }
  if (!request.LinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateLink", "Required field: LinkId, is not set");
    return DisassociateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [LinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/link-associations");
  return DisassociateLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateLinkOutcomeCallable NetworkManagerClient::DisassociateLinkCallable(const DisassociateLinkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateLink, request, m_executor.get());
}

void NetworkManagerClient::DisassociateLinkAsync(const DisassociateLinkRequest& request, const DisassociateLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateLink, request, handler, context, m_executor.get());
}

DisassociateTransitGatewayConnectPeerOutcome NetworkManagerClient::DisassociateTransitGatewayConnectPeer(const DisassociateTransitGatewayConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateTransitGatewayConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateTransitGatewayConnectPeer", "Required field: GlobalNetworkId, is not set");
    return DisassociateTransitGatewayConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.TransitGatewayConnectPeerArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateTransitGatewayConnectPeer", "Required field: TransitGatewayConnectPeerArn, is not set");
    return DisassociateTransitGatewayConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TransitGatewayConnectPeerArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateTransitGatewayConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-connect-peer-associations/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTransitGatewayConnectPeerArn());
  return DisassociateTransitGatewayConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateTransitGatewayConnectPeerOutcomeCallable NetworkManagerClient::DisassociateTransitGatewayConnectPeerCallable(const DisassociateTransitGatewayConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateTransitGatewayConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::DisassociateTransitGatewayConnectPeerAsync(const DisassociateTransitGatewayConnectPeerRequest& request, const DisassociateTransitGatewayConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateTransitGatewayConnectPeer, request, handler, context, m_executor.get());
}

ExecuteCoreNetworkChangeSetOutcome NetworkManagerClient::ExecuteCoreNetworkChangeSet(const ExecuteCoreNetworkChangeSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExecuteCoreNetworkChangeSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExecuteCoreNetworkChangeSet", "Required field: CoreNetworkId, is not set");
    return ExecuteCoreNetworkChangeSetOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  if (!request.PolicyVersionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExecuteCoreNetworkChangeSet", "Required field: PolicyVersionId, is not set");
    return ExecuteCoreNetworkChangeSetOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PolicyVersionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExecuteCoreNetworkChangeSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-change-sets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPolicyVersionId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/execute");
  return ExecuteCoreNetworkChangeSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ExecuteCoreNetworkChangeSetOutcomeCallable NetworkManagerClient::ExecuteCoreNetworkChangeSetCallable(const ExecuteCoreNetworkChangeSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ExecuteCoreNetworkChangeSet, request, m_executor.get());
}

void NetworkManagerClient::ExecuteCoreNetworkChangeSetAsync(const ExecuteCoreNetworkChangeSetRequest& request, const ExecuteCoreNetworkChangeSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ExecuteCoreNetworkChangeSet, request, handler, context, m_executor.get());
}

GetConnectAttachmentOutcome NetworkManagerClient::GetConnectAttachment(const GetConnectAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConnectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConnectAttachment", "Required field: AttachmentId, is not set");
    return GetConnectAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConnectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return GetConnectAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConnectAttachmentOutcomeCallable NetworkManagerClient::GetConnectAttachmentCallable(const GetConnectAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConnectAttachment, request, m_executor.get());
}

void NetworkManagerClient::GetConnectAttachmentAsync(const GetConnectAttachmentRequest& request, const GetConnectAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConnectAttachment, request, handler, context, m_executor.get());
}

GetConnectPeerOutcome NetworkManagerClient::GetConnectPeer(const GetConnectPeerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ConnectPeerIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConnectPeer", "Required field: ConnectPeerId, is not set");
    return GetConnectPeerOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectPeerId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConnectPeer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectPeerId());
  return GetConnectPeerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConnectPeerOutcomeCallable NetworkManagerClient::GetConnectPeerCallable(const GetConnectPeerRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConnectPeer, request, m_executor.get());
}

void NetworkManagerClient::GetConnectPeerAsync(const GetConnectPeerRequest& request, const GetConnectPeerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConnectPeer, request, handler, context, m_executor.get());
}

GetConnectPeerAssociationsOutcome NetworkManagerClient::GetConnectPeerAssociations(const GetConnectPeerAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConnectPeerAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConnectPeerAssociations", "Required field: GlobalNetworkId, is not set");
    return GetConnectPeerAssociationsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConnectPeerAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peer-associations");
  return GetConnectPeerAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConnectPeerAssociationsOutcomeCallable NetworkManagerClient::GetConnectPeerAssociationsCallable(const GetConnectPeerAssociationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConnectPeerAssociations, request, m_executor.get());
}

void NetworkManagerClient::GetConnectPeerAssociationsAsync(const GetConnectPeerAssociationsRequest& request, const GetConnectPeerAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConnectPeerAssociations, request, handler, context, m_executor.get());
}

GetConnectionsOutcome NetworkManagerClient::GetConnections(const GetConnectionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetConnections, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetConnections", "Required field: GlobalNetworkId, is not set");
    return GetConnectionsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetConnections, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connections");
  return GetConnectionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetConnectionsOutcomeCallable NetworkManagerClient::GetConnectionsCallable(const GetConnectionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetConnections, request, m_executor.get());
}

void NetworkManagerClient::GetConnectionsAsync(const GetConnectionsRequest& request, const GetConnectionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetConnections, request, handler, context, m_executor.get());
}

GetCoreNetworkOutcome NetworkManagerClient::GetCoreNetwork(const GetCoreNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetwork", "Required field: CoreNetworkId, is not set");
    return GetCoreNetworkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  return GetCoreNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCoreNetworkOutcomeCallable NetworkManagerClient::GetCoreNetworkCallable(const GetCoreNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCoreNetwork, request, m_executor.get());
}

void NetworkManagerClient::GetCoreNetworkAsync(const GetCoreNetworkRequest& request, const GetCoreNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCoreNetwork, request, handler, context, m_executor.get());
}

GetCoreNetworkChangeEventsOutcome NetworkManagerClient::GetCoreNetworkChangeEvents(const GetCoreNetworkChangeEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCoreNetworkChangeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetworkChangeEvents", "Required field: CoreNetworkId, is not set");
    return GetCoreNetworkChangeEventsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  if (!request.PolicyVersionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetworkChangeEvents", "Required field: PolicyVersionId, is not set");
    return GetCoreNetworkChangeEventsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PolicyVersionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCoreNetworkChangeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-change-events/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPolicyVersionId());
  return GetCoreNetworkChangeEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCoreNetworkChangeEventsOutcomeCallable NetworkManagerClient::GetCoreNetworkChangeEventsCallable(const GetCoreNetworkChangeEventsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCoreNetworkChangeEvents, request, m_executor.get());
}

void NetworkManagerClient::GetCoreNetworkChangeEventsAsync(const GetCoreNetworkChangeEventsRequest& request, const GetCoreNetworkChangeEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCoreNetworkChangeEvents, request, handler, context, m_executor.get());
}

GetCoreNetworkChangeSetOutcome NetworkManagerClient::GetCoreNetworkChangeSet(const GetCoreNetworkChangeSetRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCoreNetworkChangeSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetworkChangeSet", "Required field: CoreNetworkId, is not set");
    return GetCoreNetworkChangeSetOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  if (!request.PolicyVersionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetworkChangeSet", "Required field: PolicyVersionId, is not set");
    return GetCoreNetworkChangeSetOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PolicyVersionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCoreNetworkChangeSet, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-change-sets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPolicyVersionId());
  return GetCoreNetworkChangeSetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCoreNetworkChangeSetOutcomeCallable NetworkManagerClient::GetCoreNetworkChangeSetCallable(const GetCoreNetworkChangeSetRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCoreNetworkChangeSet, request, m_executor.get());
}

void NetworkManagerClient::GetCoreNetworkChangeSetAsync(const GetCoreNetworkChangeSetRequest& request, const GetCoreNetworkChangeSetResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCoreNetworkChangeSet, request, handler, context, m_executor.get());
}

GetCoreNetworkPolicyOutcome NetworkManagerClient::GetCoreNetworkPolicy(const GetCoreNetworkPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCoreNetworkPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCoreNetworkPolicy", "Required field: CoreNetworkId, is not set");
    return GetCoreNetworkPolicyOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCoreNetworkPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-policy");
  return GetCoreNetworkPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCoreNetworkPolicyOutcomeCallable NetworkManagerClient::GetCoreNetworkPolicyCallable(const GetCoreNetworkPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCoreNetworkPolicy, request, m_executor.get());
}

void NetworkManagerClient::GetCoreNetworkPolicyAsync(const GetCoreNetworkPolicyRequest& request, const GetCoreNetworkPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCoreNetworkPolicy, request, handler, context, m_executor.get());
}

GetCustomerGatewayAssociationsOutcome NetworkManagerClient::GetCustomerGatewayAssociations(const GetCustomerGatewayAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetCustomerGatewayAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetCustomerGatewayAssociations", "Required field: GlobalNetworkId, is not set");
    return GetCustomerGatewayAssociationsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetCustomerGatewayAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/customer-gateway-associations");
  return GetCustomerGatewayAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetCustomerGatewayAssociationsOutcomeCallable NetworkManagerClient::GetCustomerGatewayAssociationsCallable(const GetCustomerGatewayAssociationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetCustomerGatewayAssociations, request, m_executor.get());
}

void NetworkManagerClient::GetCustomerGatewayAssociationsAsync(const GetCustomerGatewayAssociationsRequest& request, const GetCustomerGatewayAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetCustomerGatewayAssociations, request, handler, context, m_executor.get());
}

GetDevicesOutcome NetworkManagerClient::GetDevices(const GetDevicesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDevices", "Required field: GlobalNetworkId, is not set");
    return GetDevicesOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDevices, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/devices");
  return GetDevicesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDevicesOutcomeCallable NetworkManagerClient::GetDevicesCallable(const GetDevicesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDevices, request, m_executor.get());
}

void NetworkManagerClient::GetDevicesAsync(const GetDevicesRequest& request, const GetDevicesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDevices, request, handler, context, m_executor.get());
}

GetLinkAssociationsOutcome NetworkManagerClient::GetLinkAssociations(const GetLinkAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLinkAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetLinkAssociations", "Required field: GlobalNetworkId, is not set");
    return GetLinkAssociationsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLinkAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/link-associations");
  return GetLinkAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetLinkAssociationsOutcomeCallable NetworkManagerClient::GetLinkAssociationsCallable(const GetLinkAssociationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetLinkAssociations, request, m_executor.get());
}

void NetworkManagerClient::GetLinkAssociationsAsync(const GetLinkAssociationsRequest& request, const GetLinkAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetLinkAssociations, request, handler, context, m_executor.get());
}

GetLinksOutcome NetworkManagerClient::GetLinks(const GetLinksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetLinks", "Required field: GlobalNetworkId, is not set");
    return GetLinksOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetLinks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/links");
  return GetLinksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetLinksOutcomeCallable NetworkManagerClient::GetLinksCallable(const GetLinksRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetLinks, request, m_executor.get());
}

void NetworkManagerClient::GetLinksAsync(const GetLinksRequest& request, const GetLinksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetLinks, request, handler, context, m_executor.get());
}

GetNetworkResourceCountsOutcome NetworkManagerClient::GetNetworkResourceCounts(const GetNetworkResourceCountsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetNetworkResourceCounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetNetworkResourceCounts", "Required field: GlobalNetworkId, is not set");
    return GetNetworkResourceCountsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetNetworkResourceCounts, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-resource-count");
  return GetNetworkResourceCountsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetNetworkResourceCountsOutcomeCallable NetworkManagerClient::GetNetworkResourceCountsCallable(const GetNetworkResourceCountsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetNetworkResourceCounts, request, m_executor.get());
}

void NetworkManagerClient::GetNetworkResourceCountsAsync(const GetNetworkResourceCountsRequest& request, const GetNetworkResourceCountsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetNetworkResourceCounts, request, handler, context, m_executor.get());
}

GetNetworkResourceRelationshipsOutcome NetworkManagerClient::GetNetworkResourceRelationships(const GetNetworkResourceRelationshipsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetNetworkResourceRelationships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetNetworkResourceRelationships", "Required field: GlobalNetworkId, is not set");
    return GetNetworkResourceRelationshipsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetNetworkResourceRelationships, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-resource-relationships");
  return GetNetworkResourceRelationshipsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetNetworkResourceRelationshipsOutcomeCallable NetworkManagerClient::GetNetworkResourceRelationshipsCallable(const GetNetworkResourceRelationshipsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetNetworkResourceRelationships, request, m_executor.get());
}

void NetworkManagerClient::GetNetworkResourceRelationshipsAsync(const GetNetworkResourceRelationshipsRequest& request, const GetNetworkResourceRelationshipsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetNetworkResourceRelationships, request, handler, context, m_executor.get());
}

GetNetworkResourcesOutcome NetworkManagerClient::GetNetworkResources(const GetNetworkResourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetNetworkResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetNetworkResources", "Required field: GlobalNetworkId, is not set");
    return GetNetworkResourcesOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetNetworkResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-resources");
  return GetNetworkResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetNetworkResourcesOutcomeCallable NetworkManagerClient::GetNetworkResourcesCallable(const GetNetworkResourcesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetNetworkResources, request, m_executor.get());
}

void NetworkManagerClient::GetNetworkResourcesAsync(const GetNetworkResourcesRequest& request, const GetNetworkResourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetNetworkResources, request, handler, context, m_executor.get());
}

GetNetworkRoutesOutcome NetworkManagerClient::GetNetworkRoutes(const GetNetworkRoutesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetNetworkRoutes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetNetworkRoutes", "Required field: GlobalNetworkId, is not set");
    return GetNetworkRoutesOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetNetworkRoutes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-routes");
  return GetNetworkRoutesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetNetworkRoutesOutcomeCallable NetworkManagerClient::GetNetworkRoutesCallable(const GetNetworkRoutesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetNetworkRoutes, request, m_executor.get());
}

void NetworkManagerClient::GetNetworkRoutesAsync(const GetNetworkRoutesRequest& request, const GetNetworkRoutesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetNetworkRoutes, request, handler, context, m_executor.get());
}

GetNetworkTelemetryOutcome NetworkManagerClient::GetNetworkTelemetry(const GetNetworkTelemetryRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetNetworkTelemetry, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetNetworkTelemetry", "Required field: GlobalNetworkId, is not set");
    return GetNetworkTelemetryOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetNetworkTelemetry, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-telemetry");
  return GetNetworkTelemetryOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetNetworkTelemetryOutcomeCallable NetworkManagerClient::GetNetworkTelemetryCallable(const GetNetworkTelemetryRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetNetworkTelemetry, request, m_executor.get());
}

void NetworkManagerClient::GetNetworkTelemetryAsync(const GetNetworkTelemetryRequest& request, const GetNetworkTelemetryResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetNetworkTelemetry, request, handler, context, m_executor.get());
}

GetResourcePolicyOutcome NetworkManagerClient::GetResourcePolicy(const GetResourcePolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResourcePolicy", "Required field: ResourceArn, is not set");
    return GetResourcePolicyOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resource-policy/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return GetResourcePolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetResourcePolicyOutcomeCallable NetworkManagerClient::GetResourcePolicyCallable(const GetResourcePolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetResourcePolicy, request, m_executor.get());
}

void NetworkManagerClient::GetResourcePolicyAsync(const GetResourcePolicyRequest& request, const GetResourcePolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetResourcePolicy, request, handler, context, m_executor.get());
}

GetRouteAnalysisOutcome NetworkManagerClient::GetRouteAnalysis(const GetRouteAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetRouteAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRouteAnalysis", "Required field: GlobalNetworkId, is not set");
    return GetRouteAnalysisOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.RouteAnalysisIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetRouteAnalysis", "Required field: RouteAnalysisId, is not set");
    return GetRouteAnalysisOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [RouteAnalysisId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetRouteAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/route-analyses/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetRouteAnalysisId());
  return GetRouteAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetRouteAnalysisOutcomeCallable NetworkManagerClient::GetRouteAnalysisCallable(const GetRouteAnalysisRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetRouteAnalysis, request, m_executor.get());
}

void NetworkManagerClient::GetRouteAnalysisAsync(const GetRouteAnalysisRequest& request, const GetRouteAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetRouteAnalysis, request, handler, context, m_executor.get());
}

GetSiteToSiteVpnAttachmentOutcome NetworkManagerClient::GetSiteToSiteVpnAttachment(const GetSiteToSiteVpnAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSiteToSiteVpnAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSiteToSiteVpnAttachment", "Required field: AttachmentId, is not set");
    return GetSiteToSiteVpnAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSiteToSiteVpnAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/site-to-site-vpn-attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return GetSiteToSiteVpnAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSiteToSiteVpnAttachmentOutcomeCallable NetworkManagerClient::GetSiteToSiteVpnAttachmentCallable(const GetSiteToSiteVpnAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSiteToSiteVpnAttachment, request, m_executor.get());
}

void NetworkManagerClient::GetSiteToSiteVpnAttachmentAsync(const GetSiteToSiteVpnAttachmentRequest& request, const GetSiteToSiteVpnAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSiteToSiteVpnAttachment, request, handler, context, m_executor.get());
}

GetSitesOutcome NetworkManagerClient::GetSites(const GetSitesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSites, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSites", "Required field: GlobalNetworkId, is not set");
    return GetSitesOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSites, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sites");
  return GetSitesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSitesOutcomeCallable NetworkManagerClient::GetSitesCallable(const GetSitesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSites, request, m_executor.get());
}

void NetworkManagerClient::GetSitesAsync(const GetSitesRequest& request, const GetSitesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSites, request, handler, context, m_executor.get());
}

GetTransitGatewayConnectPeerAssociationsOutcome NetworkManagerClient::GetTransitGatewayConnectPeerAssociations(const GetTransitGatewayConnectPeerAssociationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTransitGatewayConnectPeerAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTransitGatewayConnectPeerAssociations", "Required field: GlobalNetworkId, is not set");
    return GetTransitGatewayConnectPeerAssociationsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTransitGatewayConnectPeerAssociations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-connect-peer-associations");
  return GetTransitGatewayConnectPeerAssociationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTransitGatewayConnectPeerAssociationsOutcomeCallable NetworkManagerClient::GetTransitGatewayConnectPeerAssociationsCallable(const GetTransitGatewayConnectPeerAssociationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetTransitGatewayConnectPeerAssociations, request, m_executor.get());
}

void NetworkManagerClient::GetTransitGatewayConnectPeerAssociationsAsync(const GetTransitGatewayConnectPeerAssociationsRequest& request, const GetTransitGatewayConnectPeerAssociationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetTransitGatewayConnectPeerAssociations, request, handler, context, m_executor.get());
}

GetTransitGatewayPeeringOutcome NetworkManagerClient::GetTransitGatewayPeering(const GetTransitGatewayPeeringRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTransitGatewayPeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.PeeringIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTransitGatewayPeering", "Required field: PeeringId, is not set");
    return GetTransitGatewayPeeringOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PeeringId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTransitGatewayPeering, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-peerings/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPeeringId());
  return GetTransitGatewayPeeringOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTransitGatewayPeeringOutcomeCallable NetworkManagerClient::GetTransitGatewayPeeringCallable(const GetTransitGatewayPeeringRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetTransitGatewayPeering, request, m_executor.get());
}

void NetworkManagerClient::GetTransitGatewayPeeringAsync(const GetTransitGatewayPeeringRequest& request, const GetTransitGatewayPeeringResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetTransitGatewayPeering, request, handler, context, m_executor.get());
}

GetTransitGatewayRegistrationsOutcome NetworkManagerClient::GetTransitGatewayRegistrations(const GetTransitGatewayRegistrationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTransitGatewayRegistrations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTransitGatewayRegistrations", "Required field: GlobalNetworkId, is not set");
    return GetTransitGatewayRegistrationsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTransitGatewayRegistrations, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-registrations");
  return GetTransitGatewayRegistrationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTransitGatewayRegistrationsOutcomeCallable NetworkManagerClient::GetTransitGatewayRegistrationsCallable(const GetTransitGatewayRegistrationsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetTransitGatewayRegistrations, request, m_executor.get());
}

void NetworkManagerClient::GetTransitGatewayRegistrationsAsync(const GetTransitGatewayRegistrationsRequest& request, const GetTransitGatewayRegistrationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetTransitGatewayRegistrations, request, handler, context, m_executor.get());
}

GetTransitGatewayRouteTableAttachmentOutcome NetworkManagerClient::GetTransitGatewayRouteTableAttachment(const GetTransitGatewayRouteTableAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTransitGatewayRouteTableAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTransitGatewayRouteTableAttachment", "Required field: AttachmentId, is not set");
    return GetTransitGatewayRouteTableAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTransitGatewayRouteTableAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-route-table-attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return GetTransitGatewayRouteTableAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTransitGatewayRouteTableAttachmentOutcomeCallable NetworkManagerClient::GetTransitGatewayRouteTableAttachmentCallable(const GetTransitGatewayRouteTableAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetTransitGatewayRouteTableAttachment, request, m_executor.get());
}

void NetworkManagerClient::GetTransitGatewayRouteTableAttachmentAsync(const GetTransitGatewayRouteTableAttachmentRequest& request, const GetTransitGatewayRouteTableAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetTransitGatewayRouteTableAttachment, request, handler, context, m_executor.get());
}

GetVpcAttachmentOutcome NetworkManagerClient::GetVpcAttachment(const GetVpcAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetVpcAttachment", "Required field: AttachmentId, is not set");
    return GetVpcAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpc-attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return GetVpcAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetVpcAttachmentOutcomeCallable NetworkManagerClient::GetVpcAttachmentCallable(const GetVpcAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetVpcAttachment, request, m_executor.get());
}

void NetworkManagerClient::GetVpcAttachmentAsync(const GetVpcAttachmentRequest& request, const GetVpcAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetVpcAttachment, request, handler, context, m_executor.get());
}

ListAttachmentsOutcome NetworkManagerClient::ListAttachments(const ListAttachmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListAttachments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListAttachments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attachments");
  return ListAttachmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListAttachmentsOutcomeCallable NetworkManagerClient::ListAttachmentsCallable(const ListAttachmentsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListAttachments, request, m_executor.get());
}

void NetworkManagerClient::ListAttachmentsAsync(const ListAttachmentsRequest& request, const ListAttachmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListAttachments, request, handler, context, m_executor.get());
}

ListConnectPeersOutcome NetworkManagerClient::ListConnectPeers(const ListConnectPeersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListConnectPeers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListConnectPeers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connect-peers");
  return ListConnectPeersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListConnectPeersOutcomeCallable NetworkManagerClient::ListConnectPeersCallable(const ListConnectPeersRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListConnectPeers, request, m_executor.get());
}

void NetworkManagerClient::ListConnectPeersAsync(const ListConnectPeersRequest& request, const ListConnectPeersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListConnectPeers, request, handler, context, m_executor.get());
}

ListCoreNetworkPolicyVersionsOutcome NetworkManagerClient::ListCoreNetworkPolicyVersions(const ListCoreNetworkPolicyVersionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCoreNetworkPolicyVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListCoreNetworkPolicyVersions", "Required field: CoreNetworkId, is not set");
    return ListCoreNetworkPolicyVersionsOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCoreNetworkPolicyVersions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-policy-versions");
  return ListCoreNetworkPolicyVersionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListCoreNetworkPolicyVersionsOutcomeCallable NetworkManagerClient::ListCoreNetworkPolicyVersionsCallable(const ListCoreNetworkPolicyVersionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListCoreNetworkPolicyVersions, request, m_executor.get());
}

void NetworkManagerClient::ListCoreNetworkPolicyVersionsAsync(const ListCoreNetworkPolicyVersionsRequest& request, const ListCoreNetworkPolicyVersionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListCoreNetworkPolicyVersions, request, handler, context, m_executor.get());
}

ListCoreNetworksOutcome NetworkManagerClient::ListCoreNetworks(const ListCoreNetworksRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListCoreNetworks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListCoreNetworks, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks");
  return ListCoreNetworksOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListCoreNetworksOutcomeCallable NetworkManagerClient::ListCoreNetworksCallable(const ListCoreNetworksRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListCoreNetworks, request, m_executor.get());
}

void NetworkManagerClient::ListCoreNetworksAsync(const ListCoreNetworksRequest& request, const ListCoreNetworksResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListCoreNetworks, request, handler, context, m_executor.get());
}

ListOrganizationServiceAccessStatusOutcome NetworkManagerClient::ListOrganizationServiceAccessStatus(const ListOrganizationServiceAccessStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListOrganizationServiceAccessStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListOrganizationServiceAccessStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/organizations/service-access");
  return ListOrganizationServiceAccessStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListOrganizationServiceAccessStatusOutcomeCallable NetworkManagerClient::ListOrganizationServiceAccessStatusCallable(const ListOrganizationServiceAccessStatusRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListOrganizationServiceAccessStatus, request, m_executor.get());
}

void NetworkManagerClient::ListOrganizationServiceAccessStatusAsync(const ListOrganizationServiceAccessStatusRequest& request, const ListOrganizationServiceAccessStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListOrganizationServiceAccessStatus, request, handler, context, m_executor.get());
}

ListPeeringsOutcome NetworkManagerClient::ListPeerings(const ListPeeringsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListPeerings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListPeerings, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/peerings");
  return ListPeeringsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListPeeringsOutcomeCallable NetworkManagerClient::ListPeeringsCallable(const ListPeeringsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListPeerings, request, m_executor.get());
}

void NetworkManagerClient::ListPeeringsAsync(const ListPeeringsRequest& request, const ListPeeringsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListPeerings, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome NetworkManagerClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable NetworkManagerClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void NetworkManagerClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

PutCoreNetworkPolicyOutcome NetworkManagerClient::PutCoreNetworkPolicy(const PutCoreNetworkPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutCoreNetworkPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutCoreNetworkPolicy", "Required field: CoreNetworkId, is not set");
    return PutCoreNetworkPolicyOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutCoreNetworkPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-policy");
  return PutCoreNetworkPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutCoreNetworkPolicyOutcomeCallable NetworkManagerClient::PutCoreNetworkPolicyCallable(const PutCoreNetworkPolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutCoreNetworkPolicy, request, m_executor.get());
}

void NetworkManagerClient::PutCoreNetworkPolicyAsync(const PutCoreNetworkPolicyRequest& request, const PutCoreNetworkPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutCoreNetworkPolicy, request, handler, context, m_executor.get());
}

PutResourcePolicyOutcome NetworkManagerClient::PutResourcePolicy(const PutResourcePolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutResourcePolicy", "Required field: ResourceArn, is not set");
    return PutResourcePolicyOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutResourcePolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resource-policy/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return PutResourcePolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

PutResourcePolicyOutcomeCallable NetworkManagerClient::PutResourcePolicyCallable(const PutResourcePolicyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(PutResourcePolicy, request, m_executor.get());
}

void NetworkManagerClient::PutResourcePolicyAsync(const PutResourcePolicyRequest& request, const PutResourcePolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(PutResourcePolicy, request, handler, context, m_executor.get());
}

RegisterTransitGatewayOutcome NetworkManagerClient::RegisterTransitGateway(const RegisterTransitGatewayRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterTransitGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RegisterTransitGateway", "Required field: GlobalNetworkId, is not set");
    return RegisterTransitGatewayOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterTransitGateway, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/transit-gateway-registrations");
  return RegisterTransitGatewayOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RegisterTransitGatewayOutcomeCallable NetworkManagerClient::RegisterTransitGatewayCallable(const RegisterTransitGatewayRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(RegisterTransitGateway, request, m_executor.get());
}

void NetworkManagerClient::RegisterTransitGatewayAsync(const RegisterTransitGatewayRequest& request, const RegisterTransitGatewayResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(RegisterTransitGateway, request, handler, context, m_executor.get());
}

RejectAttachmentOutcome NetworkManagerClient::RejectAttachment(const RejectAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RejectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RejectAttachment", "Required field: AttachmentId, is not set");
    return RejectAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RejectAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/reject");
  return RejectAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RejectAttachmentOutcomeCallable NetworkManagerClient::RejectAttachmentCallable(const RejectAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(RejectAttachment, request, m_executor.get());
}

void NetworkManagerClient::RejectAttachmentAsync(const RejectAttachmentRequest& request, const RejectAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(RejectAttachment, request, handler, context, m_executor.get());
}

RestoreCoreNetworkPolicyVersionOutcome NetworkManagerClient::RestoreCoreNetworkPolicyVersion(const RestoreCoreNetworkPolicyVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreCoreNetworkPolicyVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RestoreCoreNetworkPolicyVersion", "Required field: CoreNetworkId, is not set");
    return RestoreCoreNetworkPolicyVersionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  if (!request.PolicyVersionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RestoreCoreNetworkPolicyVersion", "Required field: PolicyVersionId, is not set");
    return RestoreCoreNetworkPolicyVersionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [PolicyVersionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreCoreNetworkPolicyVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-network-policy-versions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetPolicyVersionId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restore");
  return RestoreCoreNetworkPolicyVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RestoreCoreNetworkPolicyVersionOutcomeCallable NetworkManagerClient::RestoreCoreNetworkPolicyVersionCallable(const RestoreCoreNetworkPolicyVersionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(RestoreCoreNetworkPolicyVersion, request, m_executor.get());
}

void NetworkManagerClient::RestoreCoreNetworkPolicyVersionAsync(const RestoreCoreNetworkPolicyVersionRequest& request, const RestoreCoreNetworkPolicyVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(RestoreCoreNetworkPolicyVersion, request, handler, context, m_executor.get());
}

StartOrganizationServiceAccessUpdateOutcome NetworkManagerClient::StartOrganizationServiceAccessUpdate(const StartOrganizationServiceAccessUpdateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartOrganizationServiceAccessUpdate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartOrganizationServiceAccessUpdate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/organizations/service-access");
  return StartOrganizationServiceAccessUpdateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartOrganizationServiceAccessUpdateOutcomeCallable NetworkManagerClient::StartOrganizationServiceAccessUpdateCallable(const StartOrganizationServiceAccessUpdateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartOrganizationServiceAccessUpdate, request, m_executor.get());
}

void NetworkManagerClient::StartOrganizationServiceAccessUpdateAsync(const StartOrganizationServiceAccessUpdateRequest& request, const StartOrganizationServiceAccessUpdateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartOrganizationServiceAccessUpdate, request, handler, context, m_executor.get());
}

StartRouteAnalysisOutcome NetworkManagerClient::StartRouteAnalysis(const StartRouteAnalysisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartRouteAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartRouteAnalysis", "Required field: GlobalNetworkId, is not set");
    return StartRouteAnalysisOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartRouteAnalysis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/route-analyses");
  return StartRouteAnalysisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartRouteAnalysisOutcomeCallable NetworkManagerClient::StartRouteAnalysisCallable(const StartRouteAnalysisRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartRouteAnalysis, request, m_executor.get());
}

void NetworkManagerClient::StartRouteAnalysisAsync(const StartRouteAnalysisRequest& request, const StartRouteAnalysisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartRouteAnalysis, request, handler, context, m_executor.get());
}

TagResourceOutcome NetworkManagerClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable NetworkManagerClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void NetworkManagerClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome NetworkManagerClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable NetworkManagerClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void NetworkManagerClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateConnectionOutcome NetworkManagerClient::UpdateConnection(const UpdateConnectionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateConnection", "Required field: GlobalNetworkId, is not set");
    return UpdateConnectionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.ConnectionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateConnection", "Required field: ConnectionId, is not set");
    return UpdateConnectionOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ConnectionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateConnection, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/connections/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetConnectionId());
  return UpdateConnectionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateConnectionOutcomeCallable NetworkManagerClient::UpdateConnectionCallable(const UpdateConnectionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateConnection, request, m_executor.get());
}

void NetworkManagerClient::UpdateConnectionAsync(const UpdateConnectionRequest& request, const UpdateConnectionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateConnection, request, handler, context, m_executor.get());
}

UpdateCoreNetworkOutcome NetworkManagerClient::UpdateCoreNetwork(const UpdateCoreNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.CoreNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateCoreNetwork", "Required field: CoreNetworkId, is not set");
    return UpdateCoreNetworkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [CoreNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateCoreNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/core-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetCoreNetworkId());
  return UpdateCoreNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateCoreNetworkOutcomeCallable NetworkManagerClient::UpdateCoreNetworkCallable(const UpdateCoreNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateCoreNetwork, request, m_executor.get());
}

void NetworkManagerClient::UpdateCoreNetworkAsync(const UpdateCoreNetworkRequest& request, const UpdateCoreNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateCoreNetwork, request, handler, context, m_executor.get());
}

UpdateDeviceOutcome NetworkManagerClient::UpdateDevice(const UpdateDeviceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDevice", "Required field: GlobalNetworkId, is not set");
    return UpdateDeviceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.DeviceIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDevice", "Required field: DeviceId, is not set");
    return UpdateDeviceOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DeviceId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDevice, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/devices/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDeviceId());
  return UpdateDeviceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateDeviceOutcomeCallable NetworkManagerClient::UpdateDeviceCallable(const UpdateDeviceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateDevice, request, m_executor.get());
}

void NetworkManagerClient::UpdateDeviceAsync(const UpdateDeviceRequest& request, const UpdateDeviceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateDevice, request, handler, context, m_executor.get());
}

UpdateGlobalNetworkOutcome NetworkManagerClient::UpdateGlobalNetwork(const UpdateGlobalNetworkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGlobalNetwork", "Required field: GlobalNetworkId, is not set");
    return UpdateGlobalNetworkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGlobalNetwork, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  return UpdateGlobalNetworkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateGlobalNetworkOutcomeCallable NetworkManagerClient::UpdateGlobalNetworkCallable(const UpdateGlobalNetworkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGlobalNetwork, request, m_executor.get());
}

void NetworkManagerClient::UpdateGlobalNetworkAsync(const UpdateGlobalNetworkRequest& request, const UpdateGlobalNetworkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGlobalNetwork, request, handler, context, m_executor.get());
}

UpdateLinkOutcome NetworkManagerClient::UpdateLink(const UpdateLinkRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateLink", "Required field: GlobalNetworkId, is not set");
    return UpdateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.LinkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateLink", "Required field: LinkId, is not set");
    return UpdateLinkOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [LinkId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateLink, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/links/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetLinkId());
  return UpdateLinkOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateLinkOutcomeCallable NetworkManagerClient::UpdateLinkCallable(const UpdateLinkRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateLink, request, m_executor.get());
}

void NetworkManagerClient::UpdateLinkAsync(const UpdateLinkRequest& request, const UpdateLinkResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateLink, request, handler, context, m_executor.get());
}

UpdateNetworkResourceMetadataOutcome NetworkManagerClient::UpdateNetworkResourceMetadata(const UpdateNetworkResourceMetadataRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateNetworkResourceMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNetworkResourceMetadata", "Required field: GlobalNetworkId, is not set");
    return UpdateNetworkResourceMetadataOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateNetworkResourceMetadata", "Required field: ResourceArn, is not set");
    return UpdateNetworkResourceMetadataOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateNetworkResourceMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/network-resources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  endpointResolutionOutcome.GetResult().AddPathSegments("/metadata");
  return UpdateNetworkResourceMetadataOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateNetworkResourceMetadataOutcomeCallable NetworkManagerClient::UpdateNetworkResourceMetadataCallable(const UpdateNetworkResourceMetadataRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateNetworkResourceMetadata, request, m_executor.get());
}

void NetworkManagerClient::UpdateNetworkResourceMetadataAsync(const UpdateNetworkResourceMetadataRequest& request, const UpdateNetworkResourceMetadataResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateNetworkResourceMetadata, request, handler, context, m_executor.get());
}

UpdateSiteOutcome NetworkManagerClient::UpdateSite(const UpdateSiteRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.GlobalNetworkIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateSite", "Required field: GlobalNetworkId, is not set");
    return UpdateSiteOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [GlobalNetworkId]", false));
  }
  if (!request.SiteIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateSite", "Required field: SiteId, is not set");
    return UpdateSiteOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [SiteId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSite, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/global-networks/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetGlobalNetworkId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/sites/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetSiteId());
  return UpdateSiteOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateSiteOutcomeCallable NetworkManagerClient::UpdateSiteCallable(const UpdateSiteRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateSite, request, m_executor.get());
}

void NetworkManagerClient::UpdateSiteAsync(const UpdateSiteRequest& request, const UpdateSiteResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateSite, request, handler, context, m_executor.get());
}

UpdateVpcAttachmentOutcome NetworkManagerClient::UpdateVpcAttachment(const UpdateVpcAttachmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AttachmentIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateVpcAttachment", "Required field: AttachmentId, is not set");
    return UpdateVpcAttachmentOutcome(Aws::Client::AWSError<NetworkManagerErrors>(NetworkManagerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AttachmentId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateVpcAttachment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/vpc-attachments/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAttachmentId());
  return UpdateVpcAttachmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateVpcAttachmentOutcomeCallable NetworkManagerClient::UpdateVpcAttachmentCallable(const UpdateVpcAttachmentRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateVpcAttachment, request, m_executor.get());
}

void NetworkManagerClient::UpdateVpcAttachmentAsync(const UpdateVpcAttachmentRequest& request, const UpdateVpcAttachmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateVpcAttachment, request, handler, context, m_executor.get());
}

