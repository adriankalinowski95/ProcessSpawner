import {Component, EventEmitter, Input, OnInit, Output, ViewChild} from '@angular/core';
import { PaginationConfig } from '../../models/ElementListConfig';
import {MatPaginator, PageEvent} from "@angular/material/paginator";
import {PageConfig} from "../../models/pageConfig";

@Component({
  selector: 'app-element-list-content-paginator',
  templateUrl: './element-list-content-paginator.component.html',
  styleUrl: './element-list-content-paginator.component.scss'
})
export class ElementListContentPaginatorComponent {
  @Input() paginationConfig: PaginationConfig;
  @Output() pageChanged = new EventEmitter<PageConfig>();
  @ViewChild(MatPaginator) paginator: MatPaginator;

  constructor() {
    console.log("paginator log!");
  }

  onPageChange($event: PageEvent) {
    this.pageChanged.emit({
      length: $event.length,
      pageIndex: $event.pageIndex,
      size: $event.pageSize
    });
  }
}