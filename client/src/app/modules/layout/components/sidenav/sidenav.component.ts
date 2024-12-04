import { BreakpointObserver } from '@angular/cdk/layout';
import { ChangeDetectionStrategy, Component, Input, OnInit, ViewChild } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';

export interface MenuItem {
    title?: string;
    icon?: string;
    link?: string;
    color?: string;
  
    hideFor?: string;
  
    expanded?: boolean;
    subMenu: MenuItem[];
  }
  
  export type Menu = MenuItem[];
  
@Component({
  selector: 'app-sidenav',
  templateUrl: './sidenav.component.html',
  styleUrl: './sidenav.component.scss',
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class SidenavComponent implements OnInit {
    @Input() 
    public isExpanded: boolean = false;

    @Input() menu: Menu = [];
    
    @ViewChild(MatSidenav)
    sidenav!: MatSidenav;
    isMobile= true;
    
    constructor(private observer: BreakpointObserver) {}

    ngOnInit() {
        this.observer.observe(['(max-width: 800px)']).subscribe((screenSize) => {
            if(screenSize.matches){
                this.isMobile = true;
            } else {
                this.isMobile = false;
            }
        });
    }

    isOpened() {
        return this.isMobile ? false : this.isExpanded;
    }
}
