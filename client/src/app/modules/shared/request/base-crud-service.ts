import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { Observable } from "rxjs";

export class BaseCrudService<T extends { id: any }> {
    constructor(protected http: HttpClient, public basePath: string) {}

    getAll(): Observable<any> {
        return this.http.get(this.basePath);
    }

    getById(id: T["id"]): Observable<any> {
        return this.http.get(this.basePath + '/' + id);
    }

    create(item: T): Observable<any> {
        return this.http.post(this.basePath, item);
    }

    update(item: T): Observable<any> {
        return this.http.delete(this.basePath + '/' + item.id);
    }

    delete(id: T["id"]): Observable<any> {
        return this.http.delete(this.basePath + '/' + id);
    }
}