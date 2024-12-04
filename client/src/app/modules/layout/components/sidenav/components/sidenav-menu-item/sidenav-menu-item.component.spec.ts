import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SidenavMenuItemComponent } from './sidenav-menu-item.component';

describe('SidenavMenuItemComponent', () => {
  let component: SidenavMenuItemComponent;
  let fixture: ComponentFixture<SidenavMenuItemComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [SidenavMenuItemComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(SidenavMenuItemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
