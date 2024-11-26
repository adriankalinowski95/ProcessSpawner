import { BaseResponse } from "./base-response";

export interface BasePaginationResponse<T> extends BaseResponse {
    data: T[];
    total: number;
    pageNumber: number;
    pageSize: number;
}