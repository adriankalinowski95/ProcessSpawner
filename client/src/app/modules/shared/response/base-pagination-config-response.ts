import { BaseResponse } from "./base-response";

export interface BasePaginationConfigResponse extends BaseResponse {
    size: number;
    lenght: number;
    pageIndex: number; 
}